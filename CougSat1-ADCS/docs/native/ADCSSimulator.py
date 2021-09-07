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
import quaternion
import sys

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

magFieldStrength = 3.5e-5

coilR = 17.808406
coilL = 0.0112
coilN = 200
coilA = 68e-3 * 62e-3

rCamera = np.array([[0, 0, -1]]).T

debugVectorLocal = True
debugVectorNorm = True

EARTH_A = 6378.137
EARTH_B = 6356.752

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

def loadWMM(plot: bool = False) -> tuple[np.ndarray, float]:
  '''!@brief Load World Magnetic Model from files in folder WMM

  Data downloaded from https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#igrfgrid

  @param plot True will plot magnetic field as a 3D vector field
  @return np.ndarray, float Array of field strengths in Teslas, altitude of model
    np.ndarray(long, lat) = field vector in global XYZ coordinates
    altitude of model in km
  '''
  raw = np.zeros((360, 180, 3))
  for path in os.listdir('WMM'):
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
  return model, altitude


wmm = None
wmmAlt = None

def interpolateWMM(long: float, lat: float) -> np.ndarray:
  '''!@brief Estimate the field strength at sub degree resolution using bilinear interpolation

  @param longitude Longitude in degrees
  @param latitude Latitude in degrees
  @return np.ndarray shape=(3, 1) Field strength in Tesla
  '''
  global wmm
  if wmm is None:
    global wmmAlt
    wmm, wmmAlt = loadWMM()
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


orb = Orbital(
  "ISS",
  line1="1 25544U 98067A   21249.54028389  .00002593  00000-0  55940-4 0  9993",
    line2="2 25544  51.6453 300.4471 0003214   1.7468 140.2698 15.48606005301202")

class ADCS:
  def __init__(self, geoTarget: np.ndarray, t: float, gps: np.ndarray,
               mag: np.ndarray, gravity: np.ndarray) -> None:
    '''!@brief Initialize ADCS control algorithm

    @param geoTarget Geodetic coordinates of target (long, lat, alt), None indicates no target and to detumble
    @param t Current time
    @param gps Geodetic coordinates from GPS (long, lat, alt)
    @param mag Sensor output of magnetometer, vector of mag field
    @param gravity Sensor output of accelerometer, vector of gravity field (normalized)
    '''
    if geoTarget is not None:
      self.ecefTarget = geo2ECEF(geoTarget[0], geoTarget[1], geoTarget[2])
    else:
      self.ecefTarget = None

    self.lastT = t
    self.lastGPS = gps
    self.lastMag = mag
    self.lastGravity = gravity

    # global debugVectorLocal
    # debugVectorLocal = False

    # global debugVectorNorm
    # debugVectorNorm = False

  def compute(self, t: float, gps: np.ndarray,
              mag: np.ndarray, gravity: np.ndarray) -> tuple[np.ndarray, np.ndarray]:
    '''!@brief Compute ADCS loop given input from sensors

    @param t Current time
    @param gps Geodetic coordinates from GPS (long, lat, alt)
    @param mag Sensor output of magnetometer, vector of mag field
    @param gravity Sensor output of accelerometer, vector of gravity field (normalized)
    @return tuple[np.ndarray, np.ndarray] Coil duty cycle, debug vector
      Coil duty cycle, shape=(3) Duty cycle written to PWM output
      Debug vector, shape=(3, 1) Vector to plot for debug purposes, None will not plot
        debugVectorLocal: True assumes vector is in local reference frame, False is global
    '''
    # Step 1: Determine attitude from gps, magnetometer, and accelerometer
    # TODO

    # Step 2: Determine current angular velocity
    # TODO

    # Step 3: Compute desired magnetic field vector
    # TODO

    # Step 4: Do control loop with current omega and target error (if target is not None)
    # TODO

    # Step 5: Transform output magnetic dipole to coil duty cycles
    # TODO

    dCoil = np.array([1.0, 0.0, 0.0])
    debugVector = np.zeros((3, 1))

    self.lastT = t
    self.lastGPS = gps
    self.lastMag = mag
    self.lastGravity = gravity

    return dCoil, debugVector

def magnetorquer(N, cog: np.ndarray, center: np.ndarray,
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
               initialOmega: float = 0.1, static: bool = False) -> None:
    '''!@brief Create a Satellite simulation

    @param detumble True will not generate a target (pass None to ADCS.__init__)
    @param initialOmega Initial angular momentum with magnitude gaussian(mu=initialOmega, sigma=initialOmega/2)
    @param static True will remain stationary, linear momentum = 0
    '''
    # Initial configuration
    self.static = static

    self.batteryVoltage = np.random.uniform(3.3, 4.2)

    self.startDatetime = datetime.datetime.now()
    self.startDatetime += datetime.timedelta(days=np.random.uniform(-100, 100))
    self.geo = np.array(orb.get_lonlatalt(self.startDatetime))
    self.ecef = geo2ECEF(self.geo[0], self.geo[1], self.geo[2])

    if detumble:
      self.geoTarget = None
      self.ecefTarget = None
    else:
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

    self.results = {
      'batteryVoltage': self.batteryVoltage,
      'initialOmega': initialOmega,
      'startTime': self.startDatetime,
      'target': self.geoTarget
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
    self.l = iBody @ omega

    self.loopFreq = 10

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
    self.iCoilList = []
    self.vCoilList = []
    self.pCoilList = []
    self.wCoilList = []
    self.cameraList = []
    self.debugVectorList = []
    self.angleErrorList = []

    # ODE Euler's method limits
    self.minTStep = 1 / (self.loopFreq * 100)
    self.maxOmega = 100
    self.maxAngleStep = self.maxOmega * self.minTStep
    self.maxAcceleration = np.deg2rad(50) / 1
    self.maxTorque = np.max(
      iBody @ np.repeat([[self.maxAcceleration]], 3, axis=0))
    self.maxLStep = self.maxTorque * self.minTStep
    self.minTStepReached = False

    # Convergence thresholds
    self.omegaThreshold = np.deg2rad(1) / 1
    self.angleThreshold = np.deg2rad(1)
    self.currentThreshold = 1e-3

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
    iInv = r @ iBodyInv @ r.T

    if not self.static:
      realTime = self.startDatetime + datetime.timedelta(seconds=t)
      self.geo = np.array(orb.get_lonlatalt(realTime))

    magField = interpolateWMM(self.geo[0], self.geo[1])
    magFieldU = magField / np.linalg.norm(magField)
    self.magFieldLocal = rInv @ magField

    gravity = geo2ECEF(self.geo[0], self.geo[1], self.geo[2])
    gravity = -gravity / np.linalg.norm(gravity)
    self.gravityLocal = rInv @ gravity

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

    if self.ecefTarget is not None:
      ecef = geo2ECEF(self.geo[0], self.geo[1], self.geo[2])
      targetDelta = self.ecefTarget - ecef
      targetDelta = targetDelta / np.linalg.norm(targetDelta)
      self.targetList.append(targetDelta)

      camera = r @ rCamera
      self.cameraList.append(camera)

      cameraNorm = camera / np.linalg.norm(camera)
      dotP = np.sum(cameraNorm * targetDelta)
      angleError = np.arccos(np.clip(dotP, -1.0, 1.0))
      self.angleErrorList.append(angleError)
    else:
      angleError = 0

    # Calculate derivatives
    omega = iInv @ self.l
    omegaQ = quaternion.from_vector_part(omega, 0)[0]
    qDot = 0.5 * omegaQ * self.q

    # Save derivatives
    self.omegaList.append(omega)

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

    self.converged = True
    if omegaQAbs > self.omegaThreshold:
      self.converged = False
    if angleError > self.angleThreshold:
      self.converged = False
    if currentAbs > self.currentThreshold:
      self.converged = False

    return tStep

  def run(self, durationMax: float = 300) -> dict:
    '''!@brief Run simulation until residuals reach zero or max duration

    @param durationMax Maximum duration to run before declaring system does not converge
    @return dict Collection of success parameters
    '''
    tStep = 1 / self.loopFreq
    t = self.__solveODE(0, self.minTStep)
    self.t.append(t)
    self.tStepList.append(t)

    adcs = ADCS(
        self.geoTarget,
        t,
        self.geo,
        self.magFieldLocal,
        self.gravityLocal)

    n = int(np.ceil(durationMax / tStep))

    for i in range(1, n):
      tTarget = tStep * i
      tRemaining = tTarget - t
      while t < tTarget and tRemaining > self.minTStep:  # TODO add a keyboardinterrupt
        dt = self.__solveODE(t, tRemaining)
        t += dt
        self.t.append(t)
        self.tStepList.append(dt)
        tRemaining = tTarget - t

      dCoil, self.debugVector = adcs.compute(
          t, self.geo, self.magFieldLocal, self.gravityLocal)
      dCoil = np.clip(dCoil, -1, 1)
      self.vCoil = dCoil * self.batteryVoltage
      if self.converged:
        break

    self.rList = np.array(self.rList)
    self.rInvList = np.array(self.rInvList)
    self.magFieldUList = np.array(self.magFieldUList)
    self.gravityList = np.array(self.gravityList)
    self.mList = np.array(self.mList)
    self.torqueList = np.array(self.torqueList)
    self.targetList = np.array(self.targetList)
    self.omegaList = np.array(self.omegaList)
    self.iCoilList = np.array(self.iCoilList)
    self.vCoilList = np.array(self.vCoilList)
    self.pCoilList = np.array(self.pCoilList)
    self.cameraList = np.array(self.cameraList)
    self.angleErrorList = np.array(self.angleErrorList)

    totalEnergy = sum(self.pCoilList * self.tStepList)

    self.results['converged'] = self.converged
    self.results['timeToConvergence'] = t
    self.results['averagePower'] = totalEnergy / t
    self.results['totalEnergy'] = totalEnergy
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
    ax = pyplot.figure().add_subplot(projection='3d')
    labels = ['x', 'y', 'z']
    colors = ['r', 'g', 'b']
    for i in range(3):
      x = self.rList[:, 0, i] * 0.5
      y = self.rList[:, 1, i] * 0.5
      z = self.rList[:, 2, i] * 0.5
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]],
                colors=colors[i], label=labels[i])
      ax.plot3D(x, y, z, colors[i], alpha=0.2)

    x = self.magFieldUList[:, 0, 0]
    y = self.magFieldUList[:, 1, 0]
    z = self.magFieldUList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='lime', label='Magnetic Field')
    ax.plot3D(x, y, z, 'lime', alpha=0.5)

    x = self.mList[:, 0, 0]
    y = self.mList[:, 1, 0]
    z = self.mList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='orange', label='Magnetic Dipole')
    ax.plot3D(x, y, z, 'orange', alpha=0.5)

    x = self.torqueList[:, 0, 0]
    y = self.torqueList[:, 1, 0]
    z = self.torqueList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='tab:brown', label='Torque')
    ax.plot3D(x, y, z, 'tab:brown', alpha=0.5)

    if self.geoTarget is not None:
      x = self.targetList[:, 0, 0]
      y = self.targetList[:, 1, 0]
      z = self.targetList[:, 2, 0]
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
                [z[-1]], colors='c', label='Target')
      ax.plot3D(x, y, z, 'c', alpha=0.5)

      x = self.cameraList[:, 0, 0]
      y = self.cameraList[:, 1, 0]
      z = self.cameraList[:, 2, 0]
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
                [z[-1]], colors='y', label='Camera')
      ax.plot3D(x, y, z, 'y', alpha=0.5)

    x = self.gravityList[:, 0, 0]
    y = self.gravityList[:, 1, 0]
    z = self.gravityList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='aquamarine', label='Gravity')
    ax.plot3D(x, y, z, 'aquamarine', alpha=0.5)

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
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='violet', label='Debug Vector')
    ax.plot3D(x, y, z, 'violet', alpha=0.5)

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.legend()
    ax.set_title('Global')
    ax.view_init(30, 30)

  def __plotLocal(self) -> None:
    '''!@brief Plot simulation local coordinates, satellite fixed
    '''
    ax = pyplot.figure().add_subplot(projection='3d')
    ax.quiver([0], [0], [0], [0.5], [0], [0], colors='r', label='x', alpha=0.2)
    ax.quiver([0], [0], [0], [0], [0.5], [0], colors='g', label='y', alpha=0.2)
    ax.quiver([0], [0], [0], [0], [0], [0.5], colors='b', label='z', alpha=0.2)

    magFieldU = []
    magDipole = []
    torque = []
    gravity = []
    for i in range(len(self.rList)):
      magFieldU.append(self.rInvList[i] @ self.magFieldUList[i])
      magDipole.append(self.rInvList[i] @ self.mList[i])
      torque.append(self.rInvList[i] @ self.torqueList[i])
      gravity.append(self.rInvList[i] @ self.gravityList[i])
    magFieldU = np.array(magFieldU)
    magDipole = np.array(magDipole)
    torque = np.array(torque)
    gravity = np.array(gravity)

    x = magFieldU[:, 0, 0]
    y = magFieldU[:, 1, 0]
    z = magFieldU[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]],
              colors='lime', label='Magnetic Field')
    ax.plot3D(x, y, z, 'lime', alpha=0.5)

    x = magDipole[:, 0, 0]
    y = magDipole[:, 1, 0]
    z = magDipole[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='orange', label='Magnetic Dipole')
    ax.plot3D(x, y, z, 'orange', alpha=0.5)

    x = torque[:, 0, 0]
    y = torque[:, 1, 0]
    z = torque[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='tab:brown', label='Torque')
    ax.plot3D(x, y, z, 'tab:brown', alpha=0.5)

    if self.geoTarget is not None:
      targetList = []
      for i in range(len(self.rList)):
        targetList.append(self.rInvList[i] @ self.targetList[i])
      targetList = np.array(targetList)
      x = targetList[:, 0, 0]
      y = targetList[:, 1, 0]
      z = targetList[:, 2, 0]
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
                [z[-1]], colors='c', label='Target')
      ax.plot3D(x, y, z, 'c', alpha=0.5)

      ax.quiver([0], [0], [0], [rCamera[0]], [rCamera[1]],
                [rCamera[2]], colors='y', label='Camera')

    x = gravity[:, 0, 0]
    y = gravity[:, 1, 0]
    z = gravity[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='aquamarine', label='Gravity')
    ax.plot3D(x, y, z, 'aquamarine', alpha=0.5)

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
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='violet', label='Debug Vector')
    ax.plot3D(x, y, z, 'violet', alpha=0.5)

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.legend()
    ax.set_title('Local')
    ax.view_init(30, 30)

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
    subplots[4].set_title('Target Error')
    subplots[4].axhline(y=0, color='k')
    subplots[4].set_ylabel('rad')

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
    self.mList = self.mList / norm

    norm = np.max(np.linalg.norm(self.torqueList, axis=1))
    self.torqueList = self.torqueList / norm

    self.__plotOrbit()
    self.__plotGlobal()
    self.__plotLocal()
    self.__plotTime()

    pyplot.show()

def _runnerSim(*args, **kwargs) -> dict:
  '''!@brief Multithreaded runner to execute simulation and return results

  @return dict same as Satellite.run
  '''
  sim = Satellite(*args, **kwargs)
  return sim.run()

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
      '--static',
      action='store_true',
      default=False,
      help='Remain stationary, linear momentum = 0')

  args = parser.parse_args(sys.argv[1:])

  args.initial_omega = float(args.initial_omega)
  args.j = max(1, int(args.j))

  kwargs = {
    'detumble': args.detumble,
    'initialOmega': args.initial_omega,
    'static': args.static
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
    with Pool(args.j) as p:
      results = [p.apply_async(_runnerSim, kwds=kwargs) for _ in range(args.n)]
      i = 0
      for p in results:
        results = p.get()
        converged.append(results['converged'])
        averagePower.append(results['averagePower'])
        totalEnergy.append(results['totalEnergy'])
        simDuration += results['timeToConvergence']

        if results['converged']:
          timeToConvergence.append(results['timeToConvergence'])
          print(
            f'{Fore.CYAN}[{i:{digits}}]{Fore.RESET} {Fore.GREEN}Convergence after {results["timeToConvergence"]:6.2f}s')
        else:
          print(
            f'{Fore.CYAN}[{i:{digits}}]{Fore.RESET} {Fore.RED}No convergence')
        i += 1
    irlDuration = (datetime.datetime.now() - start).total_seconds()
    print('-----------')
    success = sum(converged) / len(converged)
    if success == 1:
      print(f'{Fore.GREEN}{success * 100:6.2f}% converged')
      averageConvergence = sum(timeToConvergence) / len(timeToConvergence)
      print(f'Average time to convergence: {averageConvergence:6.2f}s')
    elif success != 0:
      print(f'{Fore.YELLOW}{success * 100:6.2f}% converged')
      averageConvergence = sum(timeToConvergence) / len(timeToConvergence)
      print(f'Average time to convergence: {averageConvergence:6.2f}s')
    else:
      print(f'{Fore.RED}{success * 100:6.2f}% converged')
    print(f'Average Power {np.average(averagePower):6.2f}W')
    print(f'Total Energy  {np.average(totalEnergy):6.2f}J')
    speed = simDuration / irlDuration
    print(f'Sim speed     {speed:6.2f}s/s')
    # TODO add collection of failed setups for individual debugging if necessary

  else:
    print(f'{Fore.CYAN}Simulation starting')
    start = datetime.datetime.now()
    sim = Satellite(**kwargs)
    results = sim.run()
    if results['converged']:
      print(
        f'{Fore.GREEN}Converged after {results["timeToConvergence"]:6.2f}s')
    else:
      print(
        f'{Fore.RED}Did not converged after {results["timeToConvergence"]:6.2f}s')
    print(f'Average Power {results["averagePower"]:6.2f}W')
    print(f'Total Energy  {results["totalEnergy"]:6.2f}J')
    irlDuration = (datetime.datetime.now() - start).total_seconds()
    simDuration = results["timeToConvergence"]
    speed = simDuration / irlDuration
    print(f'Sim speed     {speed:6.2f}s/s')
    sim.plot()


if __name__ == '__main__':
  main()
