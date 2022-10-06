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


def main(): #just for testing 
    # put code here for testing. Python and C++ functions should have same
    # outputs for identical inputs. Test them separetly from rest of ADCS code
    # before putting them in the vectorFunctions.h file



    ##Saturate and torque2Dipole seem to be working fine, still requires testing


  ##saturate tests
  # satTest1 = np.array([5,10,15])
  # satTest2 = np.array([9,20,-4])
  # satTest3 = np.array([-3,-4,9.5])
  # satTest4 = np.array([20,30,-90])

  # print(saturate(satTest1, 3, 12)) 
  # print(saturate(satTest2, 3, 12)) 
  # print(saturate(satTest3, 3, 12)) 
  # print(saturate(satTest4, 3, 12)) 

  ##findTorque test
  u = np.array([5,5,5])
  v = np.array([4,7,7])
  mag = np.array([3,2,1])

  # print(findTorque(u,v,mag))

  ##applyRodRotation test
  v2 = np.array([4,-3,4])
  rod = np.array([4,-3,4,0])

  # print(applyRodRotation(v2,rod))

  ## torque2Dipole test
  tordqueDir = np.array([5,5,5])
  mag2 = np.array([3,2,1])

  print(torque2Dipole(tordqueDir,mag2))



    #Example
  x = np.array([1,2,3])
  y = np.array([3,4,1])

  # print(rodRotation(x,y))


def rodRotation(a: np.ndarray, aT: np.ndarray):
  '''!@brief Created a rodrigues rotation vector that describes the rotation between
  the two input vectors


  @param a, 3 element input vector of initial state
  @param aT, 3 element input vector of transformed state
  @return np.ndarray, 4 element rodrgues Rotation Vector from a to aT
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

  @param a, 3 element input vector initial state
  @param aT, 3 element input vector transformed state
  @return np.ndarray, 3 element rotation vector between a and aT
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
#TODO
def applyRodRotation(v: np.ndarray,rod:np.ndarray):
  '''!@brief https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula


  @param v, 3 element vector to be transformed
  @param rod, 4 element rodrigues rotation vector
  @return np.ndarray, 3 element transformed v
  '''
  k = rod[0:3]
  theta = rod[3]
  k = k.flatten() / np.linalg.norm(k)
  v = v.flatten() / np.linalg.norm(v)

  vT = (v*np.cos(theta)) + (np.cross(k,v)*np.sin(theta)) + (k*np.dot(k,v)*(1 - np.cos(theta)))
  return vT

#TODO
def torque2Dipole(torqueDir,mag):
  '''!@brief Find dipole that generates the desired torque


  @param torqueDir, 3 element direction of torque vector
  @param mag, 3 element earth magnetic field vector
  @return np.ndarray, 3 element dipole
  '''
  torqueDir = torqueDir.flatten()
  mag = mag.flatten()

  dipoleDir = np.cross(mag,torqueDir)
  return dipoleDir

#TODO
def findTorque(u: np.ndarray, v: np.ndarray, mag: np.ndarray):
  '''!@brief Find torque that will move u to v on the shortest
  possible path.


  @param u, 3 element vector that will be moved (camera)
  @param v, 3 element vector that is the target (target)
  @return np.ndarray, 3 element torque vector that will move u to v on the shortest path
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

#TODO
def saturate(v: np.ndarray, min: float, max: float):
  '''!@brief Enforces min/max values for each element of v
  without changing the direction of v. Used to enforce the 
  voltage limits of each magnetorquer without changing the
  direction of the dipole.

  @param v, 3 element vector to enforce min/max values on
  @param min, minimum allowable value
  @param max, maximum allowable value
  @return v, 3 element vector with min/max values applied
  '''
  
  for i in range(0,v.size):
    if v[i] > max or v[i] < min:
      v = v * abs(max/v[i])
  return v














# THESE FUNCTIONS ARE ONLY HERE FOR TESTING PURPOSES AND HAVE ALREADY BEEN CONVERTED TO C++



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

main()