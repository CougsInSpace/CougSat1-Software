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

colorama.init(autoreset=True)

def plotGlobal(simData) -> None:
    '''!@brief Plot simulation global coordinates, satellite fixed
    '''
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

    #plot everything
    lines = []
    fig = pyplot.figure()
    ax = fig.add_subplot(projection='3d')
    labels = ['x', 'y', 'z']
    colors = ['r', 'g', 'b']
    for i in range(3):
      x = rList[:, 0, i] * 0.5
      y = rList[:, 1, i] * 0.5
      z = rList[:, 2, i] * 0.5
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors=colors[i])
      lines.append(ax.plot3D(x, y, z, colors[i], label=labels[i]))

    x = magFieldUList[:, 0, 0]
    y = magFieldUList[:, 1, 0]
    z = magFieldUList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='lime')
    lines.append(ax.plot3D(x, y, z, 'lime', label='Magnetic Field'))

    x = mList[:, 0, 0]
    y = mList[:, 1, 0]
    z = mList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='orange')
    lines.append(ax.plot3D(x, y, z, 'orange', label='Magnetic Dipole'))

    x = torqueList[:, 0, 0]
    y = torqueList[:, 1, 0]
    z = torqueList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='tab:brown')
    lines.append(ax.plot3D(x, y, z, 'tab:brown', label='Torque'))

    norm = np.max(np.linalg.norm(omegaList, axis=1))
    if norm == 0:
      norm = 1
    omega = omegaList / norm

    x = omega[:, 0, 0]
    y = omega[:, 1, 0]
    z = omega[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='grey')
    lines.append(ax.plot3D(x, y, z, 'grey', label='Omega'))

    x = targetOmegaList[:, 0, 0]
    y = targetOmegaList[:, 1, 0]
    z = targetOmegaList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='deeppink')
    lines.append(ax.plot3D(x, y, z, 'deeppink', label='Target Omega'))

    if geoTarget is not None:
      x = targetList[:, 0, 0]
      y = targetList[:, 1, 0]
      z = targetList[:, 2, 0]
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='c')
      lines.append(ax.plot3D(x, y, z, 'c', label='Target'))

      x = cameraList[:, 0, 0]
      y = cameraList[:, 1, 0]
      z = cameraList[:, 2, 0]
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='y')
      lines.append(ax.plot3D(x, y, z, 'y', label='Camera'))

    x = gravityList[:, 0, 0]
    y = gravityList[:, 1, 0]
    z = gravityList[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='aquamarine')
    lines.append(ax.plot3D(x, y, z, 'aquamarine', label='Gravity'))

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

    pyplot.show()

userInput = input("Enter number of .pickle file you would like to plot: ")
simData = pickle.load(open("simFile" + userInput +".pickle", "rb"))
plotGlobal(simData)