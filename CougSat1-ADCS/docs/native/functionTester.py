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

colorama.init(autoreset=True)

def latLongSun(time: datetime.datetime):
  ascDec = astronomy.sun_ra_dec(time)
  sideReal = astronomy.gmst(time)

  print(sideReal)

  asc = ascDec[0]
  dec = ascDec[1]

  if asc < 0:
    asc = asc + (2*np.pi)

  long = asc - sideReal
  if long > np.pi:
    long = np.pi - long
  lat = dec
  

  return np.array([np.degrees(lat), np.degrees(long)])

realTime = datetime.datetime.now()

# WRONG WRONG
print(realTime)
print(latLongSun(realTime))

# for i in range(24):
#   realTime = realTime + datetime.timedelta(hours=1)
#   print(realTime)
#   latLong = latLongSun(realTime)
#   print(latLong)
  
  

# def planeProject(earthMF,cameraDir,targetIdeal):
#   # uses this video for equation https://www.youtube.com/watch?v=QTcSBB3uVP0
#   planeVec = np.cross(earthMF,cameraDir)
#   normal = vectorProject(targetIdeal,planeVec)

#   return targetIdeal - normal

# def vectorProject(u,v):
#   '''
#   Vector projecttion of u onto v
#   '''
#   return (np.dot(v,u) / (np.linalg.norm(v)**2)) * v

# def rodRotation(a: np.ndarray, aT: np.ndarray):
#   '''!@brief Created a rodrigues rotation vector that describes the rotation between
#   the two input vectors


#   @param a Input vector initial state
#   @param aT Input vector transformed state
#   @return np.ndarray Rodrgues Rotation Vector from a to aT
#   '''
#   a = a.flatten()
#   aT = aT.flatten()
#   axis = np.cross(a,aT).flatten()
#   axis = axis / np.linalg.norm(axis)
#   theta = thetaError(a, aT)

#   return np.append(axis, theta)

# def applyRodRotation(v: np.ndarray,rod:np.ndarray):
#   '''!@brief https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula


#   @param v vector to be transformed
#   @param rod rodrigues rotation vector
#   @return transformed v
#   '''
#   k = rod[0:3]
#   theta = rod[3]
#   k = k.flatten() / np.linalg.norm(k)
#   v = v.flatten() / np.linalg.norm(v)

#   vT = (v*np.cos(theta)) + (np.cross(k,v)*np.sin(theta)) + (k*np.dot(k,v)*(1 - np.cos(theta)))
#   print(np.dot(v, vT))
#   return vT

# def thetaError(v1, v2):
#     v1 = v1.flatten()
#     v2 = v2.flatten()
#     #Find the unit vectors first
#     v1unit = v1 / np.linalg.norm(v1)
#     v2unit = v2 / np.linalg.norm(v2)
#     #Find the dot product between v1 and v2 unit vectors
#     dotproduct = np.dot(v1unit, v2unit)
#     #Take arccos to find the angle in radians between the two
#     anglebetween = np.arccos(dotproduct)
#     return anglebetween

# def saturate(v: np.ndarray, min: float, max: float):
#   for i in range(0,v.size):
#     if v[i] > max or v[i] < min:
#       v = v * abs(max/v[i])
#   return v

# vector = np.array([-6, 2, 2])
# print(vector)

# print(saturate(vector, -3, 3))

# a = np.array([3,-2,1])
# b = np.array([7, 12, -8])
# c = np.array([-4, 3, 10])

# a = a / np.linalg.norm(a)
# b = b / np.linalg.norm(b)
# c = c / np.linalg.norm(c)

# print(planeProject(a,b,c))

# print(np.dot(np.cross(a,b), np.cross(a,c)))


# rod = rodRotation(a,b)
# rod[3] = np.pi / 2
# tran = applyRodRotation(a,rod)
# print(b)
# print(tran)
# print(np.linalg.norm(tran))
