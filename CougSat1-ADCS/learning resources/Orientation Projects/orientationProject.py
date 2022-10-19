import sys
from turtle import end_fill
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
# import numpy_quaternion as quat
from tqdm import tqdm
import pickle
import math
import random

'''Initilization Section Start'''
# Inertia matrix
Ixx = 1.5
Ixy = 0
Ixz = 0
Iyy = -1
Iyz = 0
Izz = .2
iLocal = np.array([[Ixx, Ixy, Ixz],
                  [Ixy, Iyy, Iyz],
                  [Ixz, Iyz, Izz]]) #/ 1e6

tStep = 0.001 # in seconds
tMax = 1 # in seconds

# input 3 numbers, create torque vector
m = np.array([1, 2, 3]).reshape((3,1))
'''Initilization Section End'''

# initial conditions
l = np.array([0, 0, 0]).reshape((3,1))
omega = np.array([0, 0, 0]).reshape((3,1))
# q = quaternion.from_float_array([1, 0, 0, 0])
q = np.quaternion(1,0,0,0)
axisX = np.array([1, 0, 0]).reshape((3,1))
axisY = np.array([0, 1, 0]).reshape((3,1))
axisZ = np.array([0, 0, 1]).reshape((3,1))

# set up arrays of values to be plotted
axisListX = []
axisListY = []
axisListZ = []
torqueList = []
momentumList = []
omegaList = []
#qAxisList = []

for i in range(int(tMax/tStep)):
    # calculate current orientation of global frame relative to local
    r = quaternion.as_rotation_matrix(q) # local to global
    rInv = np.linalg.inv(r) # global to local

    # transform rotation matrix into global frame
    iGlobal = r @ iLocal @ r.T
    iInv = np.linalg.inv(iGlobal)

    # calculate new rotational momentum from torque
    l = l + m * tStep

    # calculate current angular velocity from momentum. 
    # Note: angular momentum must be conserved in the Global frame
    omega = iInv @ m

    # convert omega to quaternion so it can multiplied by the quaternion orienation
    # omegaq = quaternion.from_float_array(np.append([0], omega))
    omegaq = np.quaternion(0, omega[0], omega[1], omega[2])

    # calculate quaternion angular velocity using the formula qdot = .5 * omegaq * q
    qdot = 0.5 * omegaq * q

    # update quaternion orientation
    q = q + qdot * tStep
    axisXGlobal = r @ axisX
    axisYGlobal = r @ axisY
    axisZGlobal = r @ axisZ
    
    # append updated vectors to their respective lists to be plotted later
    axisListX.append(axisXGlobal)
    axisListY.append(axisYGlobal)
    axisListZ.append(axisZGlobal)
    torqueList.append(m)
    momentumList.append(l)
    omegaList.append(omega)


# plot everything
axisListX = np.array(axisListX)
axisListY = np.array(axisListY)
axisListZ = np.array(axisListZ)
torqueList = np.array(torqueList)
momentumList = np.array(momentumList)
omegaList = np.array(omegaList)

lines = []
fig = pyplot.figure()
ax = fig.add_subplot(projection="3d")

x = axisListX[:, 0, 0]
y = axisListX[:, 1, 0]
z = axisListX[:, 2, 0]

ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='red')
lines.append(ax.plot3D(x,y,z, 'red', label='x Axis Local'))

x = axisListY[:, 0, 0]
y = axisListY[:, 1, 0]
z = axisListY[:, 2, 0]

ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='green')
lines.append(ax.plot3D(x,y,z, 'green', label='y Axis Local'))

x = axisListZ[:, 0, 0]
y = axisListZ[:, 1, 0]
z = axisListZ[:, 2, 0]

ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='blue')
lines.append(ax.plot3D(x,y,z, 'blue', label='z Axis Local'))

norm = np.max(np.linalg.norm(torqueList, axis=1))
list = torqueList / norm

x = list[:, 0, 0]
y = list[:, 1, 0]
z = list[:, 2, 0]

ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='brown')
lines.append(ax.plot3D(x,y,z, 'brown', label='Torque'))

norm = np.max(np.linalg.norm(momentumList, axis=1))
list = momentumList / norm

x = list[:, 0, 0]
y = list[:, 1, 0]
z = list[:, 2, 0]

ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='pink')
lines.append(ax.plot3D(x,y,z, 'pink', label='Angular Momentum'))

norm = np.max(np.linalg.norm(omegaList, axis=1))
list = omegaList / norm

x = list[:, 0, 0]
y = list[:, 1, 0]
z = list[:, 2, 0]

ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='grey')
lines.append(ax.plot3D(x,y,z, 'grey', label='Angular Velocity'))

ax.set_xlim(-1, 1)
ax.set_ylim(-1, 1)
ax.set_zlim(-1, 1)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')


leg = ax.legend(fancybox=True, shadow=False)
lined = {}
for legline, origline in zip(leg.get_lines(), lines):
    legline.set_picker(True)
    lined[legline] = origline[0]
    lined[legline].set_visible(True)

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

pyplot.show()
