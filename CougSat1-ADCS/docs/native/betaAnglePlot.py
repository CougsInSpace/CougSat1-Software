from locale import normalize
import sys
from time import time_ns
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
import datetime as dt
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

# Constants
EARTH_A = 6378.137 # big radius of earth (km)
EARTH_B = 6356.752 # small radius of earth (km)

# orb = Orbital(
#   "Iridium 137",
#     line1 = "1 43076U 17083G   22073.57996760  .00000086  00000-0  23787-4 0  9997",
#       line2 = "2 43076  86.3937 252.1455 0001977 100.5738 259.5680 14.34217242221202")

orb = Orbital(
  "ISS",
    line1="1 25544U 98067A   21249.54028389  .00002593  00000-0  55940-4 0  9993",
      line2="2 25544  51.6453 300.4471 0003214   1.7468 140.2698 15.48606005301202")

def main():
  # starTime = dt.datetime.now()
  startTime = dt.datetime(2022, 1, 1) # set manual date
  time=startTime
  tStep = dt.timedelta(days=1)
  angleList = []
  timeList = []

  sign = 1

  for i in range(1,2*365):
    ecef1, _ = orb.get_position(time, normalize=True)
    ecef1 = np.array(ecef1).reshape((3,1))

    ecef2, _ = orb.get_position(time + dt.timedelta(minutes=15), normalize=True)
    ecef2 = np.array(ecef2).reshape((3,1))

    norm = np.cross(ecef1.flatten(), ecef2.flatten())


    sun = sunDir(time)
    proj = planeProject(ecef1, ecef2, sun)

    if thetaError(sun, norm) > np.pi/2:
      sign = -1
    else:
      sign = 1

    angle = (thetaError(sun,proj)/np.pi)*180 * sign

    angleList.append(angle)
    timeList.append(i)

    time += tStep
  #end

  max = np.amax(angleList)
  maxi = np.argmax(angleList)

  print("Maximum beta angle: ", max, " degrees")
  print("Date of max beta angle: ", (startTime + dt.timedelta(days=maxi*1.0)))



  np.array(angleList)
  np.array(timeList)

  _, subplots = pyplot.subplots(2, 1, sharex=True)
  subplots[-1].set_xlabel('Time (days)')

  subplots[0].plot(timeList, angleList, 'b')
  pyplot.show()








# useful functions

def sunDir(time: dt.datetime):
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

def vectorProject(u,v):
  '''!@brief Projects u onto v


  @param u vector to project
  @param v vector to be projected onto
  @return vector projection of u onto v
  '''
  return (np.dot(v,u) / (np.linalg.norm(v)**2)) * v

main()
