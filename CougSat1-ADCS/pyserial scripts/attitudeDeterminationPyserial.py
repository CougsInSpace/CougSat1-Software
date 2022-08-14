from curses import baudrate
from socket import timeout
import serial
import numpy as np
import matplotlib.pyplot as plt

ser = serial.Serial(
    port = 'COM5',
    baudrate = 9600,
    timeout = 1,
)

fig = plt.figure()
ax = plt.axes(projection='3d')

magList = []
for i in range(200):
    mag = [0,0,0]
    for j in range(3):
        line = ser.readline()
        string = line.decode().strip()
        if len(string) > 2:
            mag[j] = float(string)
    magList.append(mag)
    plotList = np.array(magList)

    x = plotList[:, 0]
    y = plotList[:, 1]
    z = plotList[:, 2]
    plt.cla()
    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.quiver([0],[0],[0], [x[-1]], [y[-1]], [z[-1]])
    plt.pause(.05)
# magList = np.array(magList)
# print(magList)
# print(magList[:,0])
# print(magList[:,1])
# print(magList[:,2])

# plotting
# fig = plt.figure()
# ax = plt.axes(projection='3d')
# x = magList[:, 0]
# y =  magList[:, 1]
# z = magList[:, 2]
# ax.quiver([0],[0],[0], [x[-1]], [y[-1]], [z[-1]])
# ax.plot3D(x,y,z, 'green', label='Mag')
# ax.set_xlim(-1, 1)
# ax.set_ylim(-1, 1)
# ax.set_zlim(-1, 1)
# ax.set_xlabel('x')
# ax.set_ylabel('y')
# ax.set_zlabel('z')

ser.close()
# plt.show()



