import sys

if sys.version_info[0] < 3:
    raise Exception("Min python version 3.8")
if sys.version_info[1] < 8:
    raise Exception("Min python version 3.8 ")
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

colorama.init(autoreset=True)


def plotGlobal(simData) -> None:
    """!@brief Plot simulation global coordinates, satellite fixed"""
    # extract data from tuple
    rList = simData[0]
    magFieldUList = simData[1]
    mList = simData[2]
    torqueList = simData[3]
    omegaList = simData[4]
    targetOmegaList = simData[5]
    targetList = simData[6]
    cameraList = simData[7]
    gravityList = simData[8]
    debugVectorList = simData[9]
    geoTarget = simData[10]
    debugVectorLocal = simData[11]

    # normalize certain vectors for easier viewing
    debugVectorList = np.array(debugVectorList)
    if True:
        norm = np.max(np.linalg.norm(debugVectorList, axis=1))
        if norm != 0:
            debugVectorList = debugVectorList / norm

    norm = np.max(np.linalg.norm(mList, axis=1))
    if norm != 0:
        mList = mList / norm

    norm = np.max(np.linalg.norm(torqueList, axis=1))
    if norm != 0:
        torqueList = torqueList / norm

    norm = np.max(np.linalg.norm(targetOmegaList, axis=1))
    if norm != 0:
        targetOmegaList = targetOmegaList / norm

    # plot everything
    lines = []
    fig = pyplot.figure()
    ax = fig.add_subplot(projection="3d")
    labels = ["x", "y", "z"]
    colors = ["r", "g", "b"]
    for i in range(3):
        x = rList[:, 0, i] * 0.5
        y = rList[:, 1, i] * 0.5
        z = rList[:, 2, i] * 0.5
        ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors=colors[i])
        lines.append(ax.plot3D(x, y, z, colors[i], label=labels[i]))

    x = magFieldUList[:, 0, 0]
    y = magFieldUList[:, 1, 0]
    z = magFieldUList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors="lime")
    lines.append(ax.plot3D(x, y, z, "lime", label="Magnetic Field"))

    x = mList[:, 0, 0]
    y = mList[:, 1, 0]
    z = mList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors="orange")
    lines.append(ax.plot3D(x, y, z, "orange", label="Magnetic Dipole"))

    x = torqueList[:, 0, 0]
    y = torqueList[:, 1, 0]
    z = torqueList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors="tab:brown")
    lines.append(ax.plot3D(x, y, z, "tab:brown", label="Torque"))

    norm = np.max(np.linalg.norm(omegaList, axis=1))
    if norm == 0:
        norm = 1
    omega = omegaList / norm

    x = omega[:, 0, 0]
    y = omega[:, 1, 0]
    z = omega[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors="grey")
    lines.append(ax.plot3D(x, y, z, "grey", label="Omega"))

    x = targetOmegaList[:, 0, 0]
    y = targetOmegaList[:, 1, 0]
    z = targetOmegaList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors="deeppink")
    lines.append(ax.plot3D(x, y, z, "deeppink", label="Target Omega"))

    if geoTarget is not None:
        x = targetList[:, 0, 0]
        y = targetList[:, 1, 0]
        z = targetList[:, 2, 0]
        ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors="c")
        lines.append(ax.plot3D(x, y, z, "c", label="Target"))

        x = cameraList[:, 0, 0]
        y = cameraList[:, 1, 0]
        z = cameraList[:, 2, 0]
        ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors="y")
        lines.append(ax.plot3D(x, y, z, "y", label="Camera"))

    x = gravityList[:, 0, 0]
    y = gravityList[:, 1, 0]
    z = gravityList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors="aquamarine")
    lines.append(ax.plot3D(x, y, z, "aquamarine", label="Gravity"))

    if debugVectorLocal:
        debugV = []
        for i in range(len(rList)):
            debugV.append(rList[i] @ debugVectorList[i])
        debugV = np.array(debugV)
    else:
        debugV = debugVectorList
    x = debugV[:, 0, 0]
    y = debugV[:, 1, 0]
    z = debugV[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors="violet")
    lines.append(ax.plot3D(x, y, z, "violet", label="Debug Vector"))

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")
    ax.set_title("Global")
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

    fig.canvas.mpl_connect("pick_event", on_pick)

    pyplot.show()


simData = pickle.load(open("simFile791.pickle", "rb"))
plotGlobal(simData)

# def latLongSun(time: datetime.datetime):
#   ascDec = astronomy.sun_ra_dec(time)
#   # sideReal = astronomy.gmst(time)

#   asc = ascDec[0]
#   dec = ascDec[1]

#   # if asc < 0:
#   #   asc = asc + (2*np.pi)

#   # print(asc)
#   # print(dec)
#   # long = asc - sideReal
#   # if long > np.pi:
#   #   long = np.pi - long
#   # lat = dec

#   # long = asc - (2*np.pi)


#   return np.array([np.degrees(dec), np.degrees(asc)])

# realTime = datetime.datetime.now()

# # print(realTime)
# # print(latLongSun(realTime))

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
