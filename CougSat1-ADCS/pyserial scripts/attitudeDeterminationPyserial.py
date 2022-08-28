from curses import baudrate
from socket import timeout
import serial
import numpy as np
import matplotlib.pyplot as plt
import quaternion

ser = serial.Serial(
    port = 'COM5',
    baudrate = 9600,
    timeout = 1
)

filteredq = quaternion.from_float_array([1,0,0,0])
qUncertainty = quaternion.from_float_array([1,0,0,0])

startPoint = True
while startPoint:
    line = ser.readline()
    string = line.decode().strip()
    if string == "x":
        startPoint = False
        print("Plotting started")

# Vector plotting
fig = plt.figure()
ax = fig.add_subplot(projection='3d')

# Orientation plotting
axes = np.array([[1,0,0], [0,1,0], [0,0,1]])
for i in range(10000):
    vec1 = [0,0,0]
    for j in range(3):
        line = ser.readline()
        string = line.decode().strip()
        vec1[j] = float(string)

    vec2 = [0,0,0]
    for j in range(3):
        line = ser.readline()
        string = line.decode().strip()
        vec2[j] = float(string)

    vec3 = [0,0,0,0]
    for j in range(4):
        line = ser.readline()
        string = line.decode().strip()
        vec3[j] = float(string)
    ser.readline() # move past "x" in serial output

    q = quaternion.from_float_array(vec3)
    r = quaternion.as_rotation_matrix(q)

    axesT = axes @ r

    plt.cla()
    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')

    ax.quiver(0, 0, 0, axes[0,:], axes[1,:], axes[2,:])
    ax.quiver(0, 0, 0, axesT[0,:], axesT[1,:], axesT[2,:], colors="red")
    ax.quiver(0, 0, 0, vec1[0], vec1[1], vec1[2], colors="yellow")
    ax.quiver(0, 0, 0, vec2[0], vec2[1], vec2[2], colors="green")

    plt.pause(.05)





# Plotting one vector
# dataList = []
# for i in range(1000):
#     vec = [0,0,0]
#     for j in range(3):
#         line = ser.readline()
#         string = line.decode().strip()
#         if len(string) > 2:
#             vec[j] = float(string)
            
#     dataList.append(vec)
#     plotList = np.array(dataList)

#     x = plotList[:, 0]
#     y = plotList[:, 1]
#     z = plotList[:, 2]
#     plt.cla()
#     ax.set_xlim(-1, 1)
#     ax.set_ylim(-1, 1)
#     ax.set_zlim(-1, 1)
#     ax.set_xlabel('x')
#     ax.set_ylabel('y')
#     ax.set_zlabel('z')
#     ax.quiver([0],[0],[0], [x[-1]], [y[-1]], [z[-1]])
#     plt.pause(.05)

# Plotting two vectors
# data1List = []
# data2List = []
# for i in range(1000):
#     vec1 = [0,0,0]
#     for j in range(3):
#         line = ser.readline()
#         string = line.decode().strip()
#         vec1[j] = float(string)
#     data1List.append(vec1)

#     vec2 = [0,0,0]
#     for j in range(3):
#         line = ser.readline()
#         string = line.decode().strip()
#         vec2[j] = float(string)
#     data2List.append(vec2)

#     plotList = np.array(data1List)

#     plt.cla()
#     ax.set_xlim(-1, 1)
#     ax.set_ylim(-1, 1)
#     ax.set_zlim(-1, 1)
#     ax.set_xlabel('x')
#     ax.set_ylabel('y')
#     ax.set_zlabel('z')

#     x = plotList[:, 0]
#     y = plotList[:, 1]
#     z = plotList[:, 2]
#     ax.quiver([0],[0],[0], [x[0]], [y[0]], [z[0]], colors="yellow")
#     ax.quiver([0],[0],[0], [x[-1]], [y[-1]], [z[-1]], colors="yellow")

#     plotList = np.array(data2List)

#     x = plotList[:, 0]
#     y = plotList[:, 1]
#     z = plotList[:, 2]

#     ax.quiver([0],[0],[0], [x[0]], [y[0]], [z[0]], colors="green")
#     ax.quiver([0],[0],[0], [x[-1]], [y[-1]], [z[-1]], colors="green")
#     plt.pause(.05)

# Line plotting
# photoList = []
# for i in range(1000):
#     line = ser.readline()
#     string = line.decode().strip()
#     if len(string) > 2:
#         photoList.append(float(string))
#         # rawValue = float(string)

#     # Take average of last 10 measurements
#     # avgList = photoList[-10:] + [rawValue]
#     # photoList.append(sum(avgList) / len(avgList))
    

#     plt.clf()
#     plt.plot(range(len(photoList)), photoList)
#     plt.pause(.05)

ser.close()
# plt.show()


