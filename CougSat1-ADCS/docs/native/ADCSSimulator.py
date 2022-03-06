import sys
if sys.version_info[0] < 3:
  raise Exception('Min python version 3.8')
if sys.version_info[1] < 8:
  raise Exception('Min python version 3.8')
if sys.version_info[1] < 9:
  from typing import Tuple
  tuple = Tuple

from argparse import ArgumentParser
import colorama
from colorama import Fore
import datetime
import json
import matplotlib.pyplot as pyplot
from multiprocessing import Pool, cpu_count
import numpy as np
import os
from pyorbital.orbital import Orbital
from pyorbital import astronomy
import quaternion
from tqdm import tqdm
import pickle
import math
import random

colorama.init(autoreset=True)

# kg mm^2
Ixx = 1754.776
Ixy = 4.906
Ixz = 8.896
Iyy = 1788.534
Iyz = 10.155
Izz = 1758.399
# Ixx = 1754
# Ixy = 0.0
# Ixz = 0.0
# Iyy = Ixx
# Iyz = 0.0
# Izz = Ixx
iBody = np.array([[Ixx, Ixy, Ixz],
                  [Ixy, Iyy, Iyz],
                  [Ixz, Iyz, Izz]]) / 1e6
iBodyInv = np.linalg.inv(iBody)

coilR = 17.808406
coilL = 0.0112
coilN = 200
coilA = 68e-3 * 62e-3

rCamera = np.array([[0, 0, -1]]).T

debugVectorLocal = True
debugVectorNorm = True

EARTH_A = 6378.137
EARTH_B = 6356.752

GEO_FACTOR = 1e-3
ALT_FACTOR = 1e4

def earthRadius(latitude: float) -> float:
  '''!@brief Calculate the radius of Earth for a given latitude (at sea level)

  @param latitude Latitude of point in degrees
  @return float Earth radius at sea level in km
  '''
  rad = np.deg2rad(latitude)
  R1 = EARTH_A
  R2 = EARTH_B

  return np.sqrt(((R1**2 * np.cos(rad))**2 + (R2**2 * np.sin(rad))
                  ** 2) / ((R1 * np.cos(rad))**2 + (R2 * np.sin(rad))**2))

def geo2ECEF(longitude: float, latitude: float, altitude: float) -> np.ndarray:
  '''!@brief Convert geodetic coordinates to Earth-Centered, Earth-Fixed coordinates (XYZ)

  @param longitude Longitude in degrees
  @param latitude Latitude in degrees
  @param altitude Height above sea level in km
  @return np.ndarray shape=(3, 1) XYZ coordinates in km
  '''
  latitude = np.deg2rad(latitude)
  longitude = np.deg2rad(longitude)

  N = EARTH_A**2 / np.sqrt(EARTH_A**2 * np.cos(latitude)
                           ** 2 + EARTH_B**2 * np.sin(latitude)**2)

  nCosLat = (N + altitude) * np.cos(latitude)
  x = nCosLat * np.cos(longitude)
  y = nCosLat * np.sin(longitude)
  z = (EARTH_B**2 / EARTH_A**2 * N + altitude) * np.sin(latitude)
  return np.array([[x], [y], [z]])

def loadWMM(plot: bool = False) -> np.ndarray:
  '''!@brief Load World Magnetic Model from files in folder WMM

  Data downloaded from https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#igrfgrid

  @param plot True will plot magnetic field as a 3D vector field
  @return np.ndarray, float Array of field strengths in Teslas, altitude of model
    np.ndarray(long, lat) = field vector in global XYZ coordinates
    altitude of model in km
  '''
  cache = os.path.join('WMM', 'cache.npy')
  if os.path.exists(cache):
    return np.load(cache)
  raw = np.zeros((360, 180, 3))
  for path in os.listdir('WMM'):
    if not path.endswith('.json'):
      continue
    with open(os.path.join('WMM', path), 'r') as file:
      data = json.load(file)['result']
    for d in data:
      lat = int(d['latitude'])
      long = int(d['longitude'])
      if 'xcomponent' in d:
        raw[long, lat, 0] = d['xcomponent']
      elif 'ycomponent' in d:
        raw[long, lat, 1] = d['ycomponent']
      elif 'zcomponent' in d:
        raw[long, lat, 2] = d['zcomponent']
      altitude = d['elevation']
  raw = raw * 1e-9

  # Raw data is in local North, East, Down coordinates
  model = np.zeros((360, 180, 3, 1))
  for long in range(360):
    for lat in range(-90, 90):
      north = raw[long, lat, 0]
      east = raw[long, lat, 1]
      up = -raw[long, lat, 2]

      latR = np.deg2rad(lat)
      longR = np.deg2rad(long)

      t = np.cos(latR) * up - np.sin(latR) * north
      w = np.sin(latR) * up + np.cos(latR) * north
      u = np.cos(longR) * t - np.sin(longR) * east
      v = np.sin(longR) * t + np.cos(longR) * east

      model[long, lat] = [[u], [v], [w]]

  if plot:
    ax = pyplot.figure().add_subplot(projection='3d')

    tailX = []
    tailY = []
    tailZ = []
    tipX = []
    tipY = []
    tipZ = []
    maxB = np.max(np.linalg.norm(model, axis=2))
    for long in range(0, 360, 45):
      for lat in range(-90, 90, 10):
        tail = geo2ECEF(long, lat, altitude).T[0] / EARTH_A
        tip = model[long, lat].T[0] / maxB * 0.2

        tailX.append(tail[0])
        tailY.append(tail[1])
        tailZ.append(tail[2])
        tipX.append(tip[0])
        tipY.append(tip[1])
        tipZ.append(tip[2])
    ax.quiver(tailX, tailY, tailZ, tipX, tipY, tipZ, colors='k')

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.legend()
    ax.set_title('World Magnetic Model')

    pyplot.show()

  np.save(cache, model)
  return model


wmm = None

def interpolateWMM(long: float, lat: float) -> np.ndarray:
  '''!@brief Estimate the field strength at sub degree resolution using bilinear interpolation

  @param longitude Longitude in degrees
  @param latitude Latitude in degrees
  @return np.ndarray shape=(3, 1) Field strength in Tesla
  '''
  global wmm
  if wmm is None:
    wmm = loadWMM()
  long = long % 360
  lat = lat % 180
  iLong0 = int(np.floor(long))
  iLong1 = (iLong0 + 1) % 360
  rLong = long - iLong0
  iLat0 = int(np.floor(lat))
  iLat1 = (iLat0 + 1) % 180
  rLat = lat - iLat0

  fLat0 = (1 - rLong) * wmm[iLong0, iLat0] + rLong * wmm[iLong1, iLat0]
  fLat1 = (1 - rLong) * wmm[iLong0, iLat1] + rLong * wmm[iLong1, iLat1]

  f = (1 - rLat) * fLat0 + rLat * fLat1
  return f

def crossProductMatrix(vector: np.ndarray) -> np.ndarray:
  '''!@brief Return the skew-symmetric cross-product matrix

  v = [x, y, z]
  m = [[ 0, -z,  y],
       [ z,  0, -x],
       [-y,  x,  0]]

  @param vector Input vector
  @param np.ndarray Matrix in the form described above
  '''
  vector = vector.flatten()
  return np.array([[0, -vector[2], vector[1]],
                   [vector[2], 0, -vector[0]],
                   [-vector[1], vector[0], 0]])

def rotMatrix1(a: np.ndarray, aT: np.ndarray) -> np.ndarray:
  '''!@brief Determine the rotation matrix that satisfies the transformation from a to aT

  Assumes rotation vector is perpendicular to a and aT and rotation is positive

  @param a Input vector initial state
  @param aT Input vector transformed state
  @return np.ndarray Transformation matrix from vFrom to vTo
  '''



  a = a.flatten() / np.linalg.norm(a)
  aT = aT.flatten() / np.linalg.norm(aT)

  v = np.cross(a, aT)
  s = np.linalg.norm(v)  # sine of angle
  v = v / s
  vm = crossProductMatrix(v)
  c = np.dot(a, aT)  # cosine of angle

  return np.identity(3) + s * vm + (1 - c) * vm @ vm

def rotMatrix2(a: np.ndarray, aT: np.ndarray, b: np.ndarray, bT: np.ndarray):
  '''!@brief Determine the rotation matrix that satisfies the transformation from a to aT and b to bT

  @param a Vector A initial state
  @param aT Vector A transformed state
  @param b Vector B initial state
  @param bT Vector B transformed state
  @return np.ndarray Transformation matrix
  '''
  a = a.flatten()
  aT = aT.flatten()
  b = b.flatten()
  bT = bT.flatten()

  # The rotation axis lies on the plane with normal = delta
  aDelta = aT - a
  bDelta = bT - b

  # Intersection of planes is perpendicular to both normals
  k = np.cross(aDelta, bDelta)
  if k.all() == 0:
    return None

  return rotMatrix3(a, aT, b, bT, k, k)

def rotMatrix3(a: np.ndarray, aT: np.ndarray, b: np.ndarray,
               bT: np.ndarray, c: np.ndarray, cT: np.ndarray):
  '''!@brief Determine the rotation matrix that satisfies the transformation from a to aT, b to bT, and c to cT

  @param a Vector A initial state
  @param aT Vector A transformed state
  @param b Vector B initial state
  @param bT Vector B transformed state
  @param c Vector C initial state
  @param cT Vector C transformed state
  @return np.ndarray Transformation matrix
  '''
  a = a.flatten()
  aT = aT.flatten()
  b = b.flatten()
  bT = bT.flatten()
  c = c.flatten()
  cT = cT.flatten()

  # Solve aT = R @ a, bT = R @ a, cT = R @ c
  out = np.array([aT, bT, cT]).T
  vectorInv = np.linalg.inv([a, b, c])
  r = np.zeros((3, 3))
  r[0] = (vectorInv @ out[0].reshape((3, 1))).reshape(3)
  r[1] = (vectorInv @ out[1].reshape((3, 1))).reshape(3)
  r[2] = (vectorInv @ out[2].reshape((3, 1))).reshape(3)
  return r

def rodRotation(a: np.ndarray, aT: np.ndarray):
  '''!@brief Created a rodrigues rotation vector that describes the rotation between
  the two input vectors


  @param a Input vector initial state
  @param aT Input vector transformed state
  @return np.ndarray Rodrgues Rotation Vector from a to aT
  '''

  a = a.flatten()
  aT = aT.flatten()

  axis = np.cross(a,aT).flatten()
  if np.linalg.norm(axis) == 0:
    axis = np.array([0,0,1])
    theta = 0
  else:
    axis = axis / np.linalg.norm(axis)
    theta = thetaError(a, aT)
  
  rod = np.append(axis, theta)

  return rod

def rotationVector(a: np.ndarray, aT: np.ndarray):
  '''!@brief Create rotation vector with axis and angle of rotation.
  Similiar to Rodrigues vector but the angle is stored as the 
  magnitude of the vector instead of as a fourth element

  @param a Input vector initial state
  @param aT Input vector transformed state
  @return np.ndarray Rotation vector between a and aT
  '''
  a = a.flatten()
  aT = aT.flatten()
  
  axis = np.cross(a, aT)

  # handle case if a and aT are parallel 
  if np.linalg.norm(axis) == 0:
    axis = np.array([0,0,1])
    theta = 0
  else:
    axis = axis / np.linalg.norm(axis)
    theta = thetaError(a, aT)
  rotVec = axis * theta

  return rotVec.reshape((3,1))

def applyRodRotation(v: np.ndarray,rod:np.ndarray):
  '''!@brief https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula


  @param v vector to be transformed
  @param rod rodrigues rotation vector
  @return transformed v
  '''
  k = rod[0:3]
  theta = rod[3]
  k = k.flatten() / np.linalg.norm(k)
  v = v.flatten() / np.linalg.norm(v)

  vT = (v*np.cos(theta)) + (np.cross(k,v)*np.sin(theta)) + (k*np.dot(k,v)*(1 - np.cos(theta)))
  return vT

def thetaError(v1,v2):
    v1 = v1.flatten()
    v2 = v2.flatten()
    #Find the unit vectors first
    v1unit = v1 / np.linalg.norm(v1)
    v2unit = v2 / np.linalg.norm(v2)
    #Find the dot product between v1 and v2 unit vectors
    dotProduct = np.dot(v1unit, v2unit)

    # if statement to handle rounding errors that make
    # dotProduct > 1 when v1=v2
    if dotProduct < 1:
      #Take arccos to find the angle in radians between the two
      anglebetween = np.arccos(dotProduct)
    else:
      anglebetween = 0

    return anglebetween

def planeProject(v1,v2,u):
  '''!@brief Projects vector onto plane


  @param v1 1st vector in projection plane
  @param v2 2nd vector in projection plane
  @param u vector to project
  @return u projected onto plane formed by v1 and v2
  '''
  v1 = v1.flatten()
  v2 = v2.flatten()
  u = u.flatten()
  
  planeVec = np.cross(v1,v2)
  normal = vectorProject(u,planeVec)

  return u - normal

def torque2Dipole(torqueDir,mag):
  '''!@brief Find dipole that generates the desired torque


  @param torqueDir direction of torque
  @param mag earth magnetic field
  @return dipole
  '''
  torqueDir = torqueDir.flatten()
  mag = mag.flatten()

  dipoleDir = np.cross(mag,torqueDir)
  return dipoleDir

def planeProjectNorm(n,u):
  '''!@brief Projects vector onto using plane normal as plane
  definition

  @param n plane normal vector
  @param u vector to project
  @return u projected onto plane defined by n
  '''
  n = n.flatten()
  u = u.flatten()

  normal = vectorProject(u,n)

  return (u - normal)

def vectorProject(u,v):
  '''!@brief Projects u onto v


  @param u vector to project
  @param v vector to be projected onto
  @return vector projection of u onto v
  '''
  return (np.dot(v,u) / (np.linalg.norm(v)**2)) * v

def findTorque(u: np.ndarray, v: np.ndarray, mag: np.ndarray):
  '''!@brief Find torque that will move u to v on the shortest
  possible path.


  @param u That will be moved (camera)
  @param v Vector that is the target (target)
  @return Torque that will move u to v on the shortest path
  '''
  u = u.flatten()
  v = v.flatten()
  mag = mag.flatten()

  w1 = np.cross(u, v)
  w2 = (u + v) / 2
  norm = np.cross(w1,w2)
  torque = np.cross(norm,mag)

  #decide direction of torque
  theta = thetaErrorVec(u,v,torque)
  rod = np.append(-1*torque.flatten(),theta)
  vec = applyRodRotation(u,rod)
  if thetaError(vec,v) < .00001:
    torque = -torque
  #c2Overc1 = (-(w1[0]*mag[0]) - (w1[1]*mag[1]) - (w1[2]*mag[2])) / ((v[0]*mag[0]) + (v[1]*mag[1]) + (v[2]*mag[2]))
  return torque

def thetaErrorVec(u: np.ndarray, v: np.ndarray, axis: np.ndarray):
  '''!@brief Theta error to see how close the rotation about axis is to
  completion

  @param u Rotating vector
  @param v Target vector
  @param axis Axis about which rotation is happening
  @return Angle representing how complete rotation is
  '''
  uProj = planeProjectNorm(axis,u)
  vProj = planeProjectNorm(axis,v)

  return thetaError(uProj,vProj)

def sunDir(time: datetime.datetime):
  '''!@brief Find position of sun in Global frame

  @param time Current time
  @return sunDir Direction vector of sun in Global frame
  '''
  ascDec = astronomy.sun_ra_dec(time)
  long = np.degrees(ascDec[0])
  lat = np.degrees(ascDec[1])

  sunDir = geo2ECEF(long,lat,1)
  sunDir = sunDir / np.linalg.norm(sunDir.flatten())

  return sunDir.reshape((3,1))

def findNormal(v1,v2,u):
  v1 = v1.flatten()
  v2 = v2.flatten()

  planeNormal = np.cross(v1,v2)

  return vectorProject(u,planeNormal)

def saturate(v: np.ndarray, min: float, max: float):
  '''!@brief Enforces min/max values for each element of v
  without changing the direction of v. Used to enforce the 
  voltage limits of each magnetorquer without changing the
  direction of the dipole.

  @param v vector to enforce min/max values on
  @param min minimum allowable value
  @param max maximum allowable value
  @return v with min/max values applied
  '''
  
  for i in range(0,v.size):
    if v[i] > max or v[i] < min:
      v = v * abs(max/v[i])
  return v

def rotMatrix2Mod(a: np.ndarray, aT: np.ndarray, b: np.ndarray, bT: np.ndarray):
  '''!@brief !!!CURRENTLY DOES NOT WORK PROPERLY!!!
  Finds rotation matrix between a and b to aT and bT, but priortizes 
  the vector a. It rotates a to aT first and then finds the closest b can get to 
  bT without compromising the first rotation.

  @param a untransformed priority vector
  @param aT transformed priority vector
  @param b untransformed secondary vector
  @param bT transformed secondary vector
  @return rotation matrix from a and b to aT and bT
  '''
  a = a.flatten() / np.linalg.norm(a)
  aT = aT.flatten() / np.linalg.norm(aT)
  b = b.flatten() / np.linalg.norm(b)
  bT = bT.flatten() / np.linalg.norm(bT)

  # rotate to a first and then apply the same rotation to b
  rod = rodRotation(a, aT)
  bT2 = applyRodRotation(b, rod)

  # find normal vectors for planes of aT/bT and aT/bT2
  n1 = np.cross(aT, bT)
  n2 = np.cross(aT, bT2)

  # find the two complimentary angles between the planes
  theta1 = thetaError(n1, n2)
  theta2 = (np.pi) - theta1

  # two candidates for the second Rodrigues Rotation Vector (RRV)
  # specifically for b
  testRod1 = np.array([aT[0], aT[1], aT[2], theta1])
  testRod2 = np.array([aT[0], aT[1], aT[2], theta2])

  # apply the two RRV candidates
  bTFinal1 = applyRodRotation(bT2, testRod1)
  bTFinal2 = applyRodRotation(bT2, testRod2)

  # find error for how close to the actual bT each RRV rotated b to be
  error1 = thetaError(bT, bTFinal1)
  error2 = thetaError(bT, bTFinal2)

  # check each candidate, choose the better one
  if error1 < error2:
    bTFinal = bTFinal1
  else:
    bTFinal = bTFinal2

  print(error1)
  print(error2)

  # make rotation matrix
  return rotMatrix2(a, aT, b, bTFinal)

orb = Orbital(
  "ISS",
    line1="1 25544U 98067A   21249.54028389  .00002593  00000-0  55940-4 0  9993",
      line2="2 25544  51.6453 300.4471 0003214   1.7468 140.2698 15.48606005301202")

class ADCS:
  def __init__(self, geoTarget: np.ndarray) -> None:
    '''!@brief Initialize ADCS control algorithm

    @param geoTarget Geodetic coordinates of target (long, lat, alt), None indicates no target and to detumble
    '''

    if geoTarget is not None:
      self.loopCount = 100
      self.ecefTarget = geo2ECEF(geoTarget[0], geoTarget[1], geoTarget[2])
    else:
      self.loopCount = 1
      self.ecefTarget = None

    self.lastT = None
    self.lastSensorT = None
    self.loopIndex = 1 % self.loopCount
    #------------------------#
    self.lastControlError1 = 0
    self.lastControlError2 = 0
    self.controlErrorDerivative1 = 0
    self.controlErrorDerivative2 = 0
    #------------------------#
    self.lastAngleBetweenTargets = 1
    self.lastSunDirLocal = np.array([0,0,0])
    self.stage = 0
    self.stageTime = 0
    self.lastTargetEarthNormal = np.array([0,0,0])
    self.lastTorque = np.array([0,0,0])
    self.lastResetT = 0
    self.reset = False

    global debugVectorLocal
    debugVectorLocal = False

    # global debugVectorNorm
    # debugVectorNorm = False

  def sensorAcquition(self, t: float, gps: np.ndarray,
                      mag: np.ndarray, gravity: np.ndarray) -> None:
    '''!@brief High frequency sensor sampling loop
    Used to apply a low pass filter to raw data

    @param t Current time
    @param gps Geodetic coordinates from GPS (long, lat, alt)
    @param mag Sensor output of magnetometer, vector of mag field
    @param gravity Sensor output of accelerometer, vector of gravity field
    '''
    if self.lastSensorT is None:
      self.lastSensorT = t
      self.gps = gps
      self.mag = mag
      self.gravity = gravity
      return

    fc = 10
    w = 2 * np.pi * (t - self.lastSensorT) * fc
    alpha = w / (w + 1)

    mag = mag / np.linalg.norm(mag)
    gravity = gravity / np.linalg.norm(gravity)

    self.gps = alpha * gps + (1 - alpha) * self.gps
    self.mag = alpha * mag + (1 - alpha) * self.mag
    self.gravity = alpha * gravity + (1 - alpha) * self.gravity

    self.mag = self.mag / np.linalg.norm(self.mag)
    self.gravity = self.gravity / np.linalg.norm(self.gravity)

    self.lastSensorT = t

  def compute(self, t: float, gps: np.ndarray,
              mag: np.ndarray, gravity: np.ndarray, sunDirLocal: np.ndarray, realTime: datetime.datetime, r: np.ndarray, omega: np.ndarray) -> tuple[np.ndarray, np.ndarray]:
    '''!@brief Compute ADCS loop given input from sensors

    @param t Current time
    @param gps Geodetic coordinates from GPS (long, lat, alt)
    @param mag Sensor output of magnetometer, vector of mag field
    @param gravity Sensor output of accelerometer, vector of gravity field
    @param r Rotation matrix from global to local (cheating, will need to be determined independently later)
    @return tuple[np.ndarray, np.ndarray] Coil target current, debug vector
      Coil target current, shape=(3) Current written to current control
      Debug vector, shape=(3, 1) Vector to plot for debug purposes, None will not plot
        debugVectorLocal: True assumes vector is in local reference frame, False is global
    '''
    rInv = np.linalg.inv(r)
    
    self.loopIndex = (self.loopIndex + 1) % self.loopCount
    self.sensorAcquition(t, gps, mag, gravity)

    # this code should probably be in sensorAcquition
    sunDirGlobal = sunDir(realTime)

    if self.loopIndex != 0:
      return None, None

    if t < 10:
      # Wait for sensors to stabilize
      return None, None

    if self.lastT is None:
      self.lastT = t
      self.lastGPS = self.gps
      self.lastMag = self.mag
      self.lastGravity = self.gravity
      self.lastR = None
      self.lastQ = None
      self.iCoil = np.zeros(3)
      return None, None

    tStep = t - self.lastT

    if self.ecefTarget is None:
      # Detumble, velocity control only
      bDot = (self.mag - self.lastMag) / tStep

      dipole = -0.1 * bDot

      # m = N * I * A
      iCoil = (dipole / coilN / coilA).reshape(3)
      
      fc = 1
      w = 2 * np.pi * tStep * fc
      alpha = w / (w + 1)

      self.iCoil = alpha * iCoil + (1 - alpha) * self.iCoil

      self.lastMag = self.mag
      self.lastT = t

      return self.iCoil, r @ dipole.reshape((3,1))

      # Step 2: Determine current angular velocity
      # TODO

    # find ecef vector of the satellite
    ecefSat = geo2ECEF(self.gps[0], self.gps[1], self.gps[2])

    magG = interpolateWMM(self.gps[0], self.gps[1])
    magG = magG / np.linalg.norm(magG)

    # # local = rInv @ global
    # # global = r @ local
    mag = mag / np.linalg.norm(mag)
    magG = magG / np.linalg.norm(magG)
    sunDirLocal = sunDirLocal / np.linalg.norm(sunDirLocal)
    sunDirGlobal = sunDirGlobal / np.linalg.norm(sunDirGlobal)

    rSensor = rotMatrix2(mag, magG, sunDirLocal, sunDirGlobal)
    # if rInv is None:
    #   # Cannot determine attitude, don't act
    #   dCoil = np.array([0.0, 0.0, 0.0])
    #   return dCoil, None
    # r = np.linalg.inv(rInv)
    # q = quaternion.from_rotation_matrix(r)
    # if self.lastR is None:
    #   # Cannot determine angular velocity, don't act
    #   self.lastT = t
    #   self.lastGPS = self.gps
    #   self.lastMag = self.mag
    #   self.lastGravity = self.gravity
    #   self.lastR = r
    #   self.lastQ = q
    #   dCoil = np.array([0.0, 0.0, 0.0])
    #   return dCoil, None
    # Step 2: Determine current angular velocity

    # From simulation, reverse this math to get omega & angular momentum
    # rInv = np.linalg.inv(r)
    # iInv = r @ self.realIBodyInv @ r.T
    # omega = iInv @ self.l
    # omegaQ = quaternion.from_vector_part(omega, 0)
    # qDot = 0.5 * omegaQ * self.q
    # q += + qDot * tStep

    # qDot = (q - self.lastQ) / tStep
    # # Interestingly, there is a real component in the quaternion if dividing
    # # by q or lastQ, average is okay though...
    # omegaQ = qDot * 2 / ((q + self.lastQ) / 2)
    # omega = (omegaQ.vec).reshape((3, 1))

    # if abs(omegaQ) > 1:
    #   # One quaternion is reversed (equivalent rotation matrix b.c. 2pi)
    #   qDot = (q + self.lastQ) / tStep
    #   omegaQ = qDot * 2 / ((q - self.lastQ) / 2)
    #   omega = (omegaQ.vec).reshape((3, 1))
    #   if abs(omegaQ) > 1:
    #     print('maybe not...')
    #     print(self.mag)
    #     print(magG)
    #     print(self.gravity)
    #     print(gravityG)
    #     print(r)
    #     print(self.lastR)
    #     print(q)
    #     print(self.lastQ)
    #     print(qDot)
    #     print(omegaQ)
    #     return None, None

    # change mag and rCamera to local global coordinates
    magGlobal = r @ mag
    rCameraGlobal = r @ rCamera


    # # Step 3: Compute desired magnetic field vector
    
    # direction we want to point
    targetGlobal = self.ecefTarget - ecefSat
    #targetIdeal = np.array([3, -8, 9])
    targetGlobal = targetGlobal / np.linalg.norm(targetGlobal)

    # checkOrtho = thetaError(magGlobal, targetGlobal)

    # rCameraGlobalProj = planeProject(magGlobal, targetGlobal, rCameraGlobal)
    # controlError1 = thetaError(rCameraGlobalProj, rCameraGlobal)
    # if (checkOrtho > np.deg2rad(80) and checkOrtho < np.deg2rad(100)) and controlError1 < .02:
    #   torqueDirIdeal = np.cross(rCameraGlobal.flatten(), targetGlobal.flatten())
    #   torque1 = planeProjectNorm(magGlobal, torqueDirIdeal)
    #   print("sdfjalkjsdf")
    # else:
    torque1 = findTorque(rCameraGlobal,targetGlobal,magGlobal)

    controlError1 = thetaErrorVec(rCameraGlobal, targetGlobal, torque1)

    controlError1Derivative = -1

    bDot = (self.mag - self.lastMag) / tStep
    torque2 = np.cross(bDot.flatten(), mag.flatten())
    torque2 = r @ torque2.reshape((3,1))
    torque1 = torque1.reshape((3,1))

    if thetaError(targetGlobal, rCameraGlobal) > .14:
      proportionalGain = 1#2.3*(1.44-np.exp(-.02*t))#.3 + .7*np.exp(-.01*t)
      derivativeGain = 18
    else:
      proportionalGain = 1
      derivativeGain = 2

    # checkOrtho = thetaError(magGlobal, targetGlobal)
    # if checkOrtho > np.deg2rad(80) and checkOrtho < np.deg2rad(100) and self.stage == 0:
    #   self.stage = 1

    # if self.stage == 0:
    #   rCameraProj = planeProject(magGlobal, targetGlobal, rCameraGlobal)
    #   idealAxis = np.cross(rCameraGlobal.flatten(), rCameraProj.flatten())
    #   controlError = thetaError(rCameraGlobal, rCameraProj)
    #   controlErrorDerivative = (controlError - self.controlErrorDerivative1) / tStep
    #   self.lastControlError1 = controlError

    #   if controlErrorDerivative < .01:
    #     self.stage = 1
    #   else:
    #     torque1 = planeProjectNorm(magGlobal, idealAxis)
    #     controlError1 = controlError
    #     derivativeGain = 10

    torque1 = torque1 / np.linalg.norm(torque1)
    torque1 = torque1*proportionalGain*controlError1
    torque2 = torque2*derivativeGain*controlError1Derivative
    torque = 1*(torque1.reshape((3,1)) + torque2.reshape((3,1)))


    #checkOrtho = thetaError(magGlobal, targetGlobal)
    # if np.linalg.norm(omega) < .005:#self.lastResetT + 510 < t:
    #   self.lastResetT = t
    #   # print("reset")

    # if self.lastResetT + 120 < t or np.linalg.norm(omega) > .05:
    #   torque = torque2
    #   # if np.linalg.norm(omega) > .05:
    #   #   print("slow down")
    #   # else: 
    #   #   print("timeout")

    checkOrtho = thetaError(magGlobal, targetGlobal)
    if checkOrtho > np.deg2rad(75) and checkOrtho < np.deg2rad(105): #and self.stage == 0:
      torque = torque2
      # print("uh oh")
    #   #self.stage = 1

    dipole = torque2Dipole(torque, magGlobal)

    iVector = dipole
    # Step 5: Transform output magnetic dipole to coil duty cycles
    # TODO

    # set coil currents
    iCoil = rInv @ iVector.reshape((3,1))
    
    #self.lastControlError1 = controlError1
    #self.lastControlError2 = controlError1
    self.lastT = t
    self.lastGPS = self.gps
    self.lastMag = self.mag
    self.lastGravity = self.gravity
    self.lastSunDirLocal = sunDirGlobal
    self.lastTorque = torque
    # self.lastR = r
    # self.lastQ = q

    return iCoil.flatten(), torque1.reshape((3,1))

def magnetorquer(N, cog: np.ndarray, iVector: np.ndarray,
                 edge1: np.ndarray, edge2: np.ndarray) -> tuple[np.ndarray, np.ndarray]:
  '''!@brief Create a magnetorquer for number of loops and geometry

  @param N Number of wire loops
  @param cog Location of center of gravity
  @param center Location of loop center
  @param edge1 Vector from center to an edge
  @param edge1 Vector from center to other edge
  @return tuple[np.array, np.array] (rCoil, iCoil)
    rCoil is vectors to wire segment center
    lCoil is vectors of wire segments
  '''
  rCoil = np.array([center + edge1, center - edge1,
                    center + edge2, center + edge2]).T
  rCoil = rCoil - np.repeat(cog, 4, axis=1)
  lCoil = N * 2 * np.array([edge2, -edge2, -edge1, edge1]).T
  return rCoil, lCoil

class Satellite:
  def __init__(self, detumble: bool = False,
               initialOmega: float = 0.1, static: bool = False, sigma: float = 0.1, pickleFailed: bool = False) -> None:
    '''!@brief Create a Satellite simulation

    @param detumble True will not generate a target (pass None to ADCS.__init__)
    @param initialOmega Initial angular momentum with magnitude gaussian(mu=initialOmega, sigma=initialOmega/2)
    @param static True will remain stationary, linear momentum = 0
    @param sigma Standard deviation to add to sensor measurements and other parameters
    '''
    # Initial configuration
    self.static = static
    self.sigma = sigma

    # Add variations
    self.realIBody = iBody * np.random.normal(1, sigma, size=(3, 3))
    self.realIBodyInv = np.linalg.inv(self.realIBody)

    self.batteryVoltage = np.random.uniform(3.3, 4.2)

    # set time to a random date
    self.startDatetime = datetime.datetime.now()
    self.startDatetime += datetime.timedelta(days=np.random.uniform(-100, 100))
    self.geo = np.array(orb.get_lonlatalt(self.startDatetime))
    self.ecef = geo2ECEF(self.geo[0], self.geo[1], self.geo[2])

    if detumble:
      self.loopFreq = 1
      self.maxDuration = 60 * 60 * 4
      self.geoTarget = None
      self.ecefTarget = None
    else:
      self.loopFreq = 100
      self.maxDuration = 60 * 5
      self.geoTarget = np.zeros(3)
      self.geoTarget[0] = (self.geo[0] +
                           np.random.uniform(-45, 45) + 180) % 360 - 180
      self.geoTarget[1] = (self.geo[1] +
                           np.random.uniform(-45, 45) + 90) % 180 - 90
      self.geoTarget[2] = 0
      self.ecefTarget = geo2ECEF(
          self.geoTarget[0],
          self.geoTarget[1],
          self.geoTarget[2])
      # magField = interpolateWMM(self.geo[0], self.geo[1])
      # target = np.cross(magField.flatten(), np.array([1,1,1]))
      # target = (target / np.linalg.norm(target)) * 6378
      # self.ecefTarget = target.reshape((3,1)) + self.ecef
      #yayaya

    self.results = {
      'batteryVoltage': self.batteryVoltage,
      'initialOmega': initialOmega,
      'startTime': self.startDatetime,
      'target': self.geoTarget,
      'sigma': self.sigma
    }

    # ODE state variables
    self.vCoil = np.zeros(3)
    self.iCoil = np.zeros(3)

    self.debugVector = np.zeros((3, 1))

    # Random initial attitude
    self.q = quaternion.from_float_array(np.random.uniform(-1, 1, 4))
    self.q = self.q / abs(self.q)

    omega = np.random.uniform(-1, 1, size=(3, 1))
    omega = omega / np.linalg.norm(omega)
    omega = omega * np.random.normal(initialOmega, initialOmega / 2)
    self.l = self.realIBody @ omega

    # Recording lists
    self.t = []
    self.tStepList = []
    self.geoList = []

    self.omegaList = []
    self.qList = []
    self.rList = []
    self.rInvList = []
    self.magFieldUList = []
    self.gravityList = []
    self.mList = []
    self.torqueList = []
    self.targetList = []
    self.targetOmegaList = []
    self.iCoilList = []
    self.vCoilList = []
    self.pCoilList = []
    self.wCoilList = []
    self.cameraList = []
    self.debugVectorList = []
    self.angleErrorList = []
    self.omegaErrorList = []

    # ODE Euler's method limits
    self.minTStep = 1 / (self.loopFreq * 100)
    self.maxOmega = 100
    self.maxAngleStep = self.maxOmega * self.minTStep
    self.maxAcceleration = np.deg2rad(50) / 1
    self.maxTorque = np.max(
      self.realIBody @ np.repeat([[self.maxAcceleration]], 3, axis=0))
    self.maxLStep = self.maxTorque * self.minTStep
    self.minTStepReached = False

    self.lastDeltaTarget = None
    self.lastTStep = None

    # Convergence thresholds
    self.omegaThreshold = np.deg2rad(1) / 1
    self.angleThreshold = np.deg2rad(1)
    self.currentThreshold = 99 # was 1e-3, effectively turned it off

    self.converged = False

  def __solveODE(self, t: float, duration: float) -> float:
    '''!@brief Solve ODE at current point up to duration in the future

    @param t Current time in seconds
    @param duration Max time to increment up to
    @return float actual time step
    '''
    # Calculate current state
    r = quaternion.as_rotation_matrix(self.q)
    rInv = np.linalg.inv(r)
    iInv = r @ self.realIBodyInv @ r.T

    if not self.static:
      realTime = self.startDatetime + datetime.timedelta(seconds=t)
      self.geo = np.array(orb.get_lonlatalt(realTime))
      self.sunDirLocal = rInv @ sunDir(realTime)
    else:
      self.sunDirLocal = rInv @ sunDir(self.startDatetime)

    magField = interpolateWMM(self.geo[0], self.geo[1])
    magFieldU = magField / np.linalg.norm(magField)
    self.magFieldLocal = rInv @ magField

    gravity = geo2ECEF(self.geo[0], self.geo[1], self.geo[2])
    gravity = -gravity / np.linalg.norm(gravity)
    self.gravityLocal = rInv @ gravity

    # find torque produced by magnetorquer
    mLocal = np.zeros(3)
    mLocal += coilN * self.iCoil[0] * np.array([coilA, 0, 0])
    mLocal += coilN * self.iCoil[1] * np.array([0, coilA, 0])
    mLocal += coilN * self.iCoil[2] * np.array([0, 0, coilA])
    m = r @ mLocal
    torque = np.cross(m, magField.reshape(3)).reshape((3, 1))
    m = m.reshape((3, 1))

    pCoil = sum(self.vCoil * self.iCoil)

    # Record current state
    self.qList.append(self.q)
    self.rList.append(r)
    self.rInvList.append(rInv)
    self.geoList.append(self.geo)
    self.magFieldUList.append(magFieldU)
    self.gravityList.append(gravity)
    self.mList.append(m)
    self.torqueList.append(torque)
    self.iCoilList.append(self.iCoil)
    self.vCoilList.append(self.vCoil)
    self.pCoilList.append(pCoil)
    self.debugVectorList.append(self.debugVector)

    # Calculate derivatives
    omega = iInv @ self.l
    omegaQ = quaternion.from_vector_part(omega, 0)[0]
    qDot = 0.5 * omegaQ * self.q

    if self.ecefTarget is not None:
      ecef = geo2ECEF(self.geo[0], self.geo[1], self.geo[2])
      deltaTarget = self.ecefTarget - ecef
      deltaTarget = deltaTarget / np.linalg.norm(deltaTarget)
      self.targetList.append(deltaTarget)

      if self.lastDeltaTarget is not None:
        targetOmegaV = np.cross(self.lastDeltaTarget, deltaTarget.reshape(3))
        norm = np.linalg.norm(targetOmegaV)
        targetOmega = np.arcsin(norm) / self.lastTStep
        targetOmegaV = targetOmegaV.reshape((3, 1)) / norm * targetOmega
        omegaError = np.linalg.norm(omega - targetOmegaV)
        self.targetOmegaList.append(targetOmegaV)
      else:
        targetOmega = None
        targetOmegaV = None
        omegaError = None
        self.targetOmegaList.append(np.zeros((3, 1)))

      camera = r @ rCamera
      self.cameraList.append(camera)

      cameraNorm = camera / np.linalg.norm(camera)
      dotP = np.sum(cameraNorm * deltaTarget)
      angleError = np.arccos(np.clip(dotP, -1.0, 1.0))

      self.lastDeltaTarget = deltaTarget.reshape(3)
    else:
      self.targetOmegaList.append(np.zeros((3, 1)))
      omegaError = None
      angleError = 0

    # Save derivatives
    self.omegaList.append(omega)
    self.angleErrorList.append(angleError)

    # Determine appropriate tStep
    tStep = duration

    torqueAbs = np.linalg.norm(torque)
    if torqueAbs != 0:
      suggestTStep = self.maxLStep / torqueAbs
      tStep = min(tStep, suggestTStep)
      if tStep < self.minTStep:
        tStep = self.minTStep
        if not self.minTStepReached:
          print(
            f'{Fore.RED} tStep has reached minimum value with torque = {torqueAbs} Nm')
          self.minTStepReached = True
    omegaQAbs = abs(omegaQ)
    if omegaQAbs != 0:
      suggestTStep = self.maxAngleStep / omegaQAbs
      tStep = min(tStep, suggestTStep)
      if tStep < self.minTStep:
        tStep = self.minTStep
        if not self.minTStepReached:
          print(
            f'{Fore.RED} tStep has reached minimum value with omega = {omegaQAbs} rad/s')
          self.minTStepReached = True

    # Euler's method
    self.l = self.l + torque * tStep
    self.q = self.q + qDot * tStep
    self.q = self.q / abs(self.q)

    # Magnetorquer RL circuit ODE is already solved
    iCoilLimit = self.vCoil / coilR
    expFactor = np.exp(-tStep * coilR / coilL)
    self.iCoil = self.iCoil * expFactor + iCoilLimit * (1 - expFactor)

    currentAbs = np.linalg.norm(self.iCoil)

    if omegaError is None:
      omegaError = omegaQAbs

    self.omegaErrorList.append(omegaError)
    self.lastTStep = tStep

    self.converged = True
    if omegaError > self.omegaThreshold:
      self.converged = False
    if angleError > self.angleThreshold:
      self.converged = False
    if currentAbs > self.currentThreshold:
      self.converged = False

    return tStep

  def run(self, progress: bool = True) -> dict:
    '''!@brief Run simulation until residuals reach zero or max duration

    @param progress True will print a progress bar
    @return dict Collection of success parameters
    '''
    tStep = 1 / self.loopFreq
    t = self.__solveODE(0, self.minTStep)
    self.t.append(t)
    self.tStepList.append(t)

    adcs = ADCS(self.geoTarget)

    n = int(np.ceil(self.maxDuration / tStep))

    if progress:
      pBar = tqdm(total=n)

    for i in range(1, n):
      if progress:
        pBar.update()
      tTarget = tStep * i
      tRemaining = tTarget - t
      while t < tTarget and tRemaining > self.minTStep:  # TODO add a keyboardinterrupt
        dt = self.__solveODE(t, tRemaining)
        t += dt
        self.t.append(t)
        self.tStepList.append(dt)
        tRemaining = tTarget - t

      # Get sensor output (add noise)
      gps = np.zeros(3)
      gps[0] = self.geo[0] + np.random.normal(0, self.sigma * GEO_FACTOR)
      gps[1] = self.geo[1] + np.random.normal(0, self.sigma * GEO_FACTOR)
      gps[2] = self.geo[2] + np.random.normal(0, self.sigma * ALT_FACTOR)
      magnetometer = self.magFieldLocal * \
          np.random.normal(1, self.sigma, size=(3, 1))
      accelerometer = self.gravityLocal * \
          np.random.normal(1, self.sigma, size=(3, 1))
      realTime = self.startDatetime + datetime.timedelta(seconds=t)
      diodes = self.sunDirLocal # add error based on experiments
      iCoilTarget, debugVector = adcs.compute(
        t, gps, magnetometer, accelerometer, diodes, realTime, self.rList[-1], self.omegaList[-1])
      if iCoilTarget is not None:
        self.vCoil = saturate(
            iCoilTarget * coilR, -self.batteryVoltage, self.batteryVoltage)
      if debugVector is not None:
        self.debugVector = debugVector
      if self.converged:
        break
    
    # check how close when failed
    # if not self.converged:
    #   print(self.omegaErrorList[-1])
    
    self.rList = np.array(self.rList)
    self.rInvList = np.array(self.rInvList)
    self.magFieldUList = np.array(self.magFieldUList)
    self.gravityList = np.array(self.gravityList)
    self.mList = np.array(self.mList)
    self.torqueList = np.array(self.torqueList)
    self.targetList = np.array(self.targetList)
    self.targetOmegaList = np.array(self.targetOmegaList)
    self.omegaList = np.array(self.omegaList)
    self.iCoilList = np.array(self.iCoilList)
    self.vCoilList = np.array(self.vCoilList)
    self.pCoilList = np.array(self.pCoilList)
    self.cameraList = np.array(self.cameraList)
    self.angleErrorList = np.array(self.angleErrorList)
    self.omegaErrorList = np.array(self.omegaErrorList)


    totalEnergy = sum(self.pCoilList * self.tStepList)

    self.results['converged'] = self.converged
    self.results['timeToConvergence'] = t
    self.results['averagePower'] = totalEnergy / t
    self.results['totalEnergy'] = totalEnergy

    if progress:
      pBar.close()

    return self.results

  def __plotOrbit(self) -> None:
    '''!@brief Plot simulation orbit, global coordinates, earth fixed
    '''
    ax = pyplot.figure().add_subplot(projection='3d')

    for long in range(0, 360, 15):
      lat = np.linspace(-90, 90, 60)
      long = np.repeat(long, len(lat))
      ecef = geo2ECEF(long, lat, 0) / EARTH_A
      x = ecef[0, 0]
      y = ecef[1, 0]
      z = ecef[2, 0]
      ax.plot3D(x, y, z, 'g', alpha=0.5)

    for lat in np.linspace(-60, 60, 5):
      long = np.linspace(0, 360, 90)
      lat = np.repeat(lat, len(long))
      ecef = geo2ECEF(long, lat, 0) / EARTH_A
      x = ecef[0, 0]
      y = ecef[1, 0]
      z = ecef[2, 0]
      ax.plot3D(x, y, z, 'g', alpha=0.5)

    geo = np.array(self.geoList)
    ecef = geo2ECEF(geo[:, 0], geo[:, 1], geo[:, 2]) / EARTH_A
    satX = ecef[0, 0]
    satY = ecef[1, 0]
    satZ = ecef[2, 0]
    ax.plot3D(satX, satY, satZ, 'k', label='Orbit')
    ax.scatter(satX[0], satY[0], satZ[0], marker='o', color='g', label='Start')
    ax.scatter(satX[-1], satY[-1], satZ[-1],
               marker='^', color='r', label='End')

    x = self.magFieldUList[:, 0, 0]
    y = self.magFieldUList[:, 1, 0]
    z = self.magFieldUList[:, 2, 0]
    ax.quiver([satX[-1]], [satY[-1]], [satZ[-1]], [x[-1]], [y[-1]],
              [z[-1]], colors='lime', label='Magnetic Field')
    ax.plot3D(satX + x, satY + y, satZ + z, 'lime', alpha=0.5)

    if self.geoTarget is not None:
      ecef = geo2ECEF(
          self.geoTarget[0],
          self.geoTarget[1],
          self.geoTarget[2]) / EARTH_A
      ax.scatter(
          ecef[0],
          ecef[1],
          ecef[2],
          marker='1',
          color='c',
          label='Target')
      x = self.targetList[:, 0, 0]
      y = self.targetList[:, 1, 0]
      z = self.targetList[:, 2, 0]
      ax.quiver([satX[-1]], [satY[-1]], [satZ[-1]], [x[-1]], [y[-1]],
                [z[-1]], colors='c')
      ax.plot3D(satX + x, satY + y, satZ + z, 'c', alpha=0.5)

      x = self.cameraList[:, 0, 0]
      y = self.cameraList[:, 1, 0]
      z = self.cameraList[:, 2, 0]
      ax.quiver([satX[-1]], [satY[-1]], [satZ[-1]], [x[-1]], [y[-1]],
                [z[-1]], colors='y', label='Camera')
      ax.plot3D(satX + x, satY + y, satZ + z, 'y', alpha=0.5)

    x = self.gravityList[:, 0, 0]
    y = self.gravityList[:, 1, 0]
    z = self.gravityList[:, 2, 0]
    ax.quiver([satX[-1]], [satY[-1]], [satZ[-1]], [x[-1]], [y[-1]],
              [z[-1]], colors='aquamarine', label='Gravity')
    ax.plot3D(satX + x, satY + y, satZ + z, 'aquamarine', alpha=0.5)

    if debugVectorLocal:
      debugV = []
      for i in range(len(self.rList)):
        debugV.append(self.rList[i] @ self.debugVectorList[i])
      debugV = np.array(debugV)
    else:
      debugV = self.debugVectorList
    x = debugV[:, 0, 0]
    y = debugV[:, 1, 0]
    z = debugV[:, 2, 0]
    ax.quiver([satX[-1]], [satY[-1]], [satZ[-1]], [x[-1]], [y[-1]],
              [z[-1]], colors='violet', label='Debug Vector')
    ax.plot3D(satX + x, satY + y, satZ + z, 'violet', alpha=0.5)

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.legend()
    ax.set_title('ECEF')
    ax.view_init(30, 30)

  def __plotGlobal(self) -> None:
    '''!@brief Plot simulation global coordinates, satellite fixed
    '''
    lines = []
    fig = pyplot.figure()
    ax = fig.add_subplot(projection='3d')
    labels = ['x', 'y', 'z']
    colors = ['r', 'g', 'b']
    for i in range(3):
      x = self.rList[:, 0, i] * 0.5
      y = self.rList[:, 1, i] * 0.5
      z = self.rList[:, 2, i] * 0.5
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors=colors[i])
      lines.append(ax.plot3D(x, y, z, colors[i], label=labels[i]))
 
    x = self.magFieldUList[:, 0, 0]
    y = self.magFieldUList[:, 1, 0]
    z = self.magFieldUList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='lime')
    lines.append(ax.plot3D(x, y, z, 'lime', label='Magnetic Field'))

    x = self.mList[:, 0, 0]
    y = self.mList[:, 1, 0]
    z = self.mList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='orange')
    lines.append(ax.plot3D(x, y, z, 'orange', label='Magnetic Dipole'))

    x = self.torqueList[:, 0, 0]
    y = self.torqueList[:, 1, 0]
    z = self.torqueList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='tab:brown')
    lines.append(ax.plot3D(x, y, z, 'tab:brown', label='Torque'))

    norm = np.max(np.linalg.norm(self.omegaList, axis=1))
    if norm == 0:
      norm = 1
    omega = self.omegaList / norm

    x = omega[:, 0, 0]
    y = omega[:, 1, 0]
    z = omega[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='grey')
    lines.append(ax.plot3D(x, y, z, 'grey', label='Omega'))

    x = self.targetOmegaList[:, 0, 0]
    y = self.targetOmegaList[:, 1, 0]
    z = self.targetOmegaList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='deeppink')
    lines.append(ax.plot3D(x, y, z, 'deeppink', label='Target Omega'))

    if self.geoTarget is not None:
      x = self.targetList[:, 0, 0]
      y = self.targetList[:, 1, 0]
      z = self.targetList[:, 2, 0]
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='c')
      lines.append(ax.plot3D(x, y, z, 'c', label='Target'))

      x = self.cameraList[:, 0, 0]
      y = self.cameraList[:, 1, 0]
      z = self.cameraList[:, 2, 0]
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='y')
      lines.append(ax.plot3D(x, y, z, 'y', label='Camera'))

    x = self.gravityList[:, 0, 0]
    y = self.gravityList[:, 1, 0]
    z = self.gravityList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='aquamarine')
    lines.append(ax.plot3D(x, y, z, 'aquamarine', label='Gravity'))

    if debugVectorLocal:
      debugV = []
      for i in range(len(self.rList)):
        debugV.append(self.rList[i] @ self.debugVectorList[i])
      debugV = np.array(debugV)
    else:
      debugV = self.debugVectorList
    x = debugV[:, 0, 0]
    y = debugV[:, 1, 0]
    z = debugV[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='violet')
    lines.append(ax.plot3D(x, y, z, 'violet', label='Debug Vector'))

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.set_title('Global')
    ax.view_init(30, 30)

    leg = ax.legend(fancybox=True, shadow=False)
    lined = {}
    for legline, origline in zip(leg.get_lines(), lines):
      legline.set_picker(True)
      lined[legline] = origline[0]
      lined[legline].set_visible(False)

    def on_pick(event):
      # On the pick event, find the original line corresponding to the legend
      # proxy line, and toggle its visibility.
      legline = event.artist
      origline = lined[legline]
      visible = not origline.get_visible()
      origline.set_visible(visible)
      # Change the alpha on the line in the legend so we can see what lines
      # have been toggled.
      legline.set_alpha(1.0 if visible else 0.2)
      fig.canvas.draw()

    fig.canvas.mpl_connect('pick_event', on_pick)

  def __plotLocal(self) -> None:
    '''!@brief Plot simulation local coordinates, satellite fixed
    '''
    lines = []
    fig = pyplot.figure()
    ax = fig.add_subplot(projection='3d')
    ax.quiver([0], [0], [0], [0.5], [0], [0], colors='r', label='x', alpha=0.2)
    ax.quiver([0], [0], [0], [0], [0.5], [0], colors='g', label='y', alpha=0.2)
    ax.quiver([0], [0], [0], [0], [0], [0.5], colors='b', label='z', alpha=0.2)

    magFieldU = []
    magDipole = []
    torque = []
    gravity = []
    omega = []
    targetOmega = []
    for i in range(len(self.rList)):
      magFieldU.append(self.rInvList[i] @ self.magFieldUList[i])
      magDipole.append(self.rInvList[i] @ self.mList[i])
      torque.append(self.rInvList[i] @ self.torqueList[i])
      gravity.append(self.rInvList[i] @ self.gravityList[i])
      omega.append(self.rInvList[i] @ self.omegaList[i])
      targetOmega.append(self.rInvList[i] @ self.targetOmegaList[i])
    magFieldU = np.array(magFieldU)
    magDipole = np.array(magDipole)
    torque = np.array(torque)
    gravity = np.array(gravity)
    omega = np.array(omega)
    targetOmega = np.array(targetOmega)

    norm = np.max(np.linalg.norm(omega, axis=1))
    if norm != 0:
      omega = omega / norm

    x = magFieldU[:, 0, 0]
    y = magFieldU[:, 1, 0]
    z = magFieldU[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='lime')
    lines.append(ax.plot3D(x, y, z, 'lime', label='Magnetic Field'))

    x = magDipole[:, 0, 0]
    y = magDipole[:, 1, 0]
    z = magDipole[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='orange')
    lines.append(ax.plot3D(x, y, z, 'orange', label='Magnetic Dipole'))

    x = torque[:, 0, 0]
    y = torque[:, 1, 0]
    z = torque[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='tab:brown')
    lines.append(ax.plot3D(x, y, z, 'tab:brown', label='Torque'))

    x = omega[:, 0, 0]
    y = omega[:, 1, 0]
    z = omega[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='grey')
    lines.append(ax.plot3D(x, y, z, 'grey', label='Omega'))

    x = targetOmega[:, 0, 0]
    y = targetOmega[:, 1, 0]
    z = targetOmega[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='deeppink')
    lines.append(ax.plot3D(x, y, z, 'deeppink', label='Target Omega'))

    if self.geoTarget is not None:
      targetList = []
      for i in range(len(self.rList)):
        targetList.append(self.rInvList[i] @ self.targetList[i])
      targetList = np.array(targetList)
      x = targetList[:, 0, 0]
      y = targetList[:, 1, 0]
      z = targetList[:, 2, 0]
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='c')
      lines.append(ax.plot3D(x, y, z, 'c', label='Target'))

      ax.quiver(
          [0], [0], [0], [
              rCamera[0]], [
              rCamera[1]], [
              rCamera[2]], colors='y', label='Camera')

    x = gravity[:, 0, 0]
    y = gravity[:, 1, 0]
    z = gravity[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='aquamarine')
    lines.append(ax.plot3D(x, y, z, 'aquamarine', label='Gravity'))

    if debugVectorLocal:
      debugV = self.debugVectorList
    else:
      debugV = []
      for i in range(len(self.rList)):
        debugV.append(self.rInvList[i] @ self.debugVectorList[i])
      debugV = np.array(debugV)
    x = debugV[:, 0, 0]
    y = debugV[:, 1, 0]
    z = debugV[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='violet')
    lines.append(ax.plot3D(x, y, z, 'violet', label='Debug Vector'))

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.set_title('Local')
    ax.view_init(30, 30)

    leg = ax.legend(fancybox=True, shadow=False)
    lined = {}
    for legline, origline in zip(leg.get_lines(), lines):
      legline.set_picker(True)
      lined[legline] = origline[0]

    def on_pick(event):
      # On the pick event, find the original line corresponding to the legend
      # proxy line, and toggle its visibility.
      legline = event.artist
      origline = lined[legline]
      lined[legline].set_visible(False)
      visible = not origline.get_visible()
      origline.set_visible(visible)
      # Change the alpha on the line in the legend so we can see what lines
      # have been toggled.
      legline.set_alpha(1.0 if visible else 0.2)
      fig.canvas.draw()

    fig.canvas.mpl_connect('pick_event', on_pick)

  def __plotTime(self) -> None:
    '''!@brief Plot simulation time domain line graphs
    '''
    # Time domain
    _, subplots = pyplot.subplots(6, 1, sharex=True)
    subplots[-1].set_xlabel('Time (s)')

    x = self.omegaList[:, 0, 0]
    y = self.omegaList[:, 1, 0]
    z = self.omegaList[:, 2, 0]
    subplots[0].plot(self.t, x, 'r')
    subplots[0].plot(self.t, y, 'g')
    subplots[0].plot(self.t, z, 'b')
    omegaMag = np.linalg.norm(self.omegaList, axis=1)
    subplots[0].plot(self.t, omegaMag, 'c')
    subplots[0].set_title('Omega')
    subplots[0].axhline(y=0, color='k')
    subplots[0].set_ylabel('rad/s')

    x = self.iCoilList[:, 0]
    y = self.iCoilList[:, 1]
    z = self.iCoilList[:, 2]
    subplots[1].plot(self.t, x, 'r')
    subplots[1].plot(self.t, y, 'g')
    subplots[1].plot(self.t, z, 'b')
    subplots[1].set_title('Coil Current')
    subplots[1].axhline(y=0, color='k')
    subplots[1].set_ylabel('A')

    x = self.vCoilList[:, 0]
    y = self.vCoilList[:, 1]
    z = self.vCoilList[:, 2]
    subplots[2].plot(self.t, x, 'r')
    subplots[2].plot(self.t, y, 'g')
    subplots[2].plot(self.t, z, 'b')
    subplots[2].set_title('Coil Voltage')
    subplots[2].axhline(y=0, color='k')
    subplots[2].set_ylabel('V')

    subplots[3].plot(self.t, self.pCoilList, 'b')
    subplots[3].set_title('Coil Power')
    subplots[3].axhline(y=0, color='k')
    subplots[3].set_ylabel('W')

    subplots[4].plot(self.t, self.angleErrorList, 'b')
    secAx = subplots[4].twinx()
    secAx.plot(self.t, self.omegaErrorList, 'm')
    subplots[4].set_title('Target Error')
    subplots[4].axhline(y=0, color='k')
    secAx.axhline(y=0, color='k')
    subplots[4].set_ylabel('rad')
    secAx.set_ylabel('rad/s')

    subplots[5].plot(self.t, self.tStepList, 'b')
    subplots[5].set_title('tStep')
    subplots[5].axhline(y=0, color='k')
    subplots[5].set_ylabel('s')

  def plot(self) -> None:
    '''!@brief Plot simulation
    '''
    self.debugVectorList = np.array(self.debugVectorList)
    if debugVectorNorm:
      norm = np.max(np.linalg.norm(self.debugVectorList, axis=1))
      if norm != 0:
        self.debugVectorList = self.debugVectorList / norm

    norm = np.max(np.linalg.norm(self.mList, axis=1))
    if norm != 0:
      self.mList = self.mList / norm

    norm = np.max(np.linalg.norm(self.torqueList, axis=1))
    if norm != 0:
      self.torqueList = self.torqueList / norm

    norm = np.max(np.linalg.norm(self.targetOmegaList, axis=1))
    if norm != 0:
      self.targetOmegaList = self.targetOmegaList / norm

    self.__plotOrbit()
    self.__plotGlobal()
    self.__plotLocal()
    self.__plotTime()

    pyplot.show() 

def _runnerSim(**kwargs) -> dict:
  '''!@brief Multithreaded runner to execute simulation and return results

  @return dict same as Satellite.run
  '''
  sim = Satellite(**kwargs)
  results = sim.run(progress=False)

  if not sim.converged and kwargs['pickleFailed']:
    simData = (sim.rList, sim.magFieldUList, sim.mList, sim.torqueList, sim.omegaList, sim.targetOmegaList,\
      sim.targetList, sim.cameraList, sim.gravityList, sim.debugVectorList,\
      sim.geoTarget, debugVectorLocal)
    simFile = open("simFile" + str(random.randint(0,10000)) +".pickle", 'wb')
    pickle.dump(simData, simFile)
    simFile.close()

  return results

def main():
  parser = ArgumentParser()
  parser.add_argument(
      '--monte-carlo',
      action='store_true',
      default=False,
      help='Run a monte carlo simulation of the ADCS')
  parser.add_argument(
      '-n',
      help='Number of monte carlo simulations to run',
      default=100)
  parser.add_argument(
      '-j',
      help='Number of threads to use for monte carlo simulation',
      default=cpu_count())
  parser.add_argument(
      '--detumble',
      action='store_true',
      default=False,
      help='Use ADCS.computeDetumble and have no target')
  parser.add_argument(
      '--initial_omega', '-w',
      default=0,
      help='Initialize angular momentum with random value up to --initial_omega rad/s')
  parser.add_argument(
      '--sigma', '-s',
      default=0.003,
      help='Add gaussian noise to sensor input by multiplying by gaussian(mu=1, sigma=[s])')
  parser.add_argument(
      '--static',
      action='store_true',
      default=False,
      help='Remain stationary, linear momentum = 0')
  parser.add_argument(
      '--pickle-failed',
      action='store_true',
      default=False,
      help='Pickle failed simulation results in monte-carlo')

  args = parser.parse_args(sys.argv[1:])

  args.initial_omega = float(args.initial_omega)
  args.sigma = float(args.sigma)
  args.j = max(1, int(args.j))

  kwargs = {
    'detumble': args.detumble,
    'initialOmega': args.initial_omega,
    'static': args.static,
    'sigma': args.sigma,
    'pickleFailed': args.pickle_failed
  }

  if args.monte_carlo:
    print(f'{Fore.CYAN}Monte Carlo simulation starting')
    args.n = int(args.n)
    digits = int(np.ceil(np.log10(args.n)))

    converged = []
    timeToConvergence = []
    averagePower = []
    totalEnergy = []

    start = datetime.datetime.now()
    simDuration = 0

    # where the simulations are run
    p = Pool(args.j) # args.j is the number of cpu threads for async
    jobs = [p.apply_async(_runnerSim, kwds=kwargs) for _ in range(args.n)]
    p.close()

     # loop through the array jobs[] to find the results 
    results = []
    for job in tqdm(jobs):
      results.append(job.get())
    i = 0
    for r in results:
      converged.append(r['converged'])
      averagePower.append(r['averagePower'])
      totalEnergy.append(r['totalEnergy'])
      simDuration += r['timeToConvergence']

      if r['converged']:
        timeToConvergence.append(r['timeToConvergence'])
      i += 1
    irlDuration = (datetime.datetime.now() - start).total_seconds()
    print('-----------')
    success = sum(converged) / len(converged)

    # print % converged in appropriate color
    if success == 1:
      print(f'{Fore.GREEN}{success * 100:8.2f}% converged')
      mean = np.mean(timeToConvergence)
      std = np.std(timeToConvergence)
      print(f'Average time to convergence: {mean:8.2f}s σ={std:8.2f}s')
    elif success != 0:
      print(f'{Fore.YELLOW}{success * 100:8.2f}% converged')
      mean = np.mean(timeToConvergence)
      std = np.std(timeToConvergence)
      print(f'Average time to convergence: {mean:8.2f}s σ={std:8.2f}s')
    else:
      print(f'{Fore.RED}{success * 100:8.2f}% converged')
    print(f'Average Power {np.average(averagePower):8.2f}W')
    print(f'Total Energy  {np.average(totalEnergy):8.2f}J')
    speed = simDuration / irlDuration
    print(f'Sim speed     {speed:8.2f}s/s')
    # TODO add collection of failed setups for individual debugging if
    # necessary

  else:
    print(f'{Fore.CYAN}Simulation starting')
    start = datetime.datetime.now()
    sim = Satellite(**kwargs)
    results = sim.run()
    if results['converged']:
      print(
        f'{Fore.GREEN}Converged after {results["timeToConvergence"]:8.2f}s')
    else:
      print(
        f'{Fore.RED}Did not converged after {results["timeToConvergence"]:8.2f}s')
    print(f'Average Power {results["averagePower"]:8.2f}W')
    print(f'Total Energy  {results["totalEnergy"]:8.2f}J')
    irlDuration = (datetime.datetime.now() - start).total_seconds()
    simDuration = results["timeToConvergence"]
    speed = simDuration / irlDuration
    print(f'Sim speed     {speed:8.2f}s/s')
    sim.plot()


if __name__ == '__main__':
  main()
