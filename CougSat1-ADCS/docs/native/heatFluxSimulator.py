from pyorbital.orbital import Orbital
from pyorbital import astronomy
import datetime as dt
import numpy as np
import quaternion
import time
import math
import matplotlib.pyplot as pyplot
from solid_works_curves import *

'''INITIALIZATION PARAMETERS START'''
# Simulation parameters
tStep = 60 # tStep in seconds
tMax = 270*60 # time simulation runs for in seconds

# Universe parameters
maxSolarConst = 1322 # W/m^2
minSolarConst = 1414 # W/m^2
albedoFac = .55 # earth albedo factor, varies between .25 and .55 based on terrain
earthBBT = 255 # kelvin. Earth black body temp
boltzmann = 5.67e-8 # W/m^2K^4
emissivity = 1 # emissivity of satellite (IR absorptivity)

# Satellite parameters
bbqOmega = 0 # rad/s WARNING: higher than .005 will neccesitate a smaller tstep
startTime = dt.datetime.now()
# startTime = dt.datetime(2022, 9, 15) # set manual date
sunFace = np.array([1,0,0]) # direction vector for direction on satellite pointing at sun
area = .1*.1 #m^2
'''INITIALIZATION PARAMETERS END'''

fileNames = ["+x", "+y", "+z", "-x", "-y", "-z"]


# Constants
EARTH_A = 6378.137 # big radius of earth (km)
EARTH_B = 6356.752 # small radius of earth (km)
EARTH_MEAN = (EARTH_A + EARTH_B) / 2

GEO_FACTOR = 1e-3
ALT_FACTOR = 1e4

# TLE for ISS
# orb = Orbital(
#   "ISS",
#     line1="1 25544U 98067A   21249.54028389  .00002593  00000-0  55940-4 0  9993",
#       line2="2 25544  51.6453 300.4471 0003214   1.7468 140.2698 15.48606005301202")

# TLE for hot case
orb = Orbital(
  "Iridium 137",
    line1 = "1 43076U 17083G   22073.57996760  .00000086  00000-0  23787-4 0  9997",
      line2 = "2 43076  86.3937 252.1455 0001977 100.5738 259.5680 14.34217242221202")

class Satellite:
  def __init__(self, Tf:float, tStep: float, startTime: dt.datetime, 
                bbqOmega: float, maxSolarConst: float, minSolarConst: float, area: float, sunFace: np.ndarray):
    self.currentTime = startTime
    self.t = 0
    self.tMax = Tf
    self.omega = np.array([0, 0, bbqOmega]) # (Global Frame)
    self.tStep = tStep
    self.area = area
    self.faceNormals = np.array([[1, 0, 0], 
                                [0, 1, 0],
                                [0, 0, 1],
                                [-1,0, 0],
                                [0,-1, 0],
                                [0, 0,-1]]).T # TODO add surface absorbtivity here (Local Frame)


    if np.linalg.norm(sunFace) != 0:
      sunDirection = sunDir(self.currentTime)
      axis = np.cross(sunFace.flatten(), sunDirection.flatten())
      angle = thetaError(sunFace, sunDirection)
      axAng = (axis / np.linalg.norm(axis)) * angle
      self.q = quaternion.from_rotation_vector(axAng)
    else:
      self.q = quaternion.from_float_array([1, 0, 0, 0])
    self.omegaq = quaternion.from_float_array(np.append([0], self.omega))
    self.qdot = .5*self.omegaq*self.q

    # flux variation throughout year
    yearFraction = toYearFraction(self.currentTime)  
    offset = toYearFraction(dt.datetime(2000, 1, 4))
    self.currentFlux = math.sqrt(((minSolarConst*math.sin(math.pi*(yearFraction - offset)))**2) + ((maxSolarConst*math.cos(math.pi*(yearFraction - offset)))**2))

    # position of satellite in orbit
    # self.latLong = np.array(orb.get_lonlatalt(self.currentTime))
    # self.ecef = geo2ECEF(self.latLong[0], self.latLong[1], self.latLong[2])
    self.ecef, _ = orb.get_position(self.currentTime, normalize=False)
    self.ecef = np.array(self.ecef).reshape((3,1))

    # lists for plotting
    self.fluxList = []
    self.netFlux = np.array([0,0,0,0,0,0])
    self.sunList = []
    self.ecefList = []
    self.shadowList = []
    self.timeList = []


  def run(self):
    # main for loop
    n = int(np.ceil(self.tMax / self.tStep))
    for i in range(1,n):
      r = quaternion.as_rotation_matrix(self.q) # local to global
      rInv = np.linalg.inv(r) # global to local
      faceNormalsGlobal = r @ self.faceNormals

      # find sun direction and check if the satellite is in shadow
      sunDirection = sunDir(self.currentTime)
      shadowBool = checkShadow(self.ecef, sunDirection)

      # solar flux
      solarFluxDir = -1*sunDirection
      solarFlux = solarFluxDir*self.currentFlux
      faceSolarFluxes = findFluxes(faceNormalsGlobal, solarFlux)
      faceSolarFluxes = faceSolarFluxes * shadowBool

      # albedo flux
      albedoFluxDir = self.ecef / np.linalg.norm(self.ecef)
      albedoFlux = albedoFluxDir * self.currentFlux
      faceAlbedofluxes = findFluxes(faceNormalsGlobal, albedoFlux)
      faceAlbedofluxes = faceAlbedofluxes * shadowBool

      # ir flux
      IRFluxDir = albedoFluxDir
      IRFlux = IRFluxDir*boltzmann*emissivity*(earthBBT**4)
      faceIRFluxes = findFluxes(faceNormalsGlobal, IRFlux)

      # sum everything up
      totalFlux = faceSolarFluxes + faceAlbedofluxes + faceIRFluxes

      # plotting lists
      self.fluxList.append(totalFlux.reshape((6,1)))
      self.netFlux = self.netFlux + (totalFlux * tStep)
      self.sunList.append(rInv @ sunDirection.reshape((3,1)))
      self.ecefList.append(rInv @ self.ecef.reshape((3,1)))
      if not shadowBool:
        self.shadowList.append(rInv @ self.ecef.reshape((3,1)))
      self.timeList.append(self.t)


      # update time
      self.currentTime = self.currentTime + dt.timedelta(seconds=self.tStep)
      self.t += self.tStep
      
      # update orientation and orbit
      self.q = self.q + (self.qdot * self.tStep)
      self.qdot = .5*self.omegaq*self.q
      self.q = self.q / abs(self.q)
      # self.latLong = np.array(orb.get_lonlatalt(self.currentTime))
      # self.ecef = geo2ECEF(self.latLong[0], self.latLong[1], self.latLong[2])
      self.ecef, _ = orb.get_position(self.currentTime, normalize=False)
      self.ecef = np.array(self.ecef).reshape((3,1))

      # calculate current flux based on time of year
      yearFraction = toYearFraction(self.currentTime)  
      offset = toYearFraction(dt.datetime(2000, 1, 4))
      self.currentFlux = math.sqrt(((minSolarConst*math.sin(math.pi*(yearFraction - offset)))**2) + ((maxSolarConst*math.cos(math.pi*(yearFraction - offset)))**2))
    # add curve data to xml
    time = np.array(self.timeList)
    fluxes = np.array(self.fluxList)
    curves = SW_curves("SolidWorks Curves")
    for i in range(0, 6):
      list = np.concatenate(([time], [fluxes[:,i,0]]), axis=0).T
      curves.add_curve("flux " + fileNames[i], list) 
    curves.generate_SW_curve_file("Heat Flux Output")




  def plotLocal(self):
    # plot everything
    self.fluxList = np.array(self.fluxList)
    netFluxNorm = self.netFlux / np.max(self.netFlux)
    self.sunList = np.array(self.sunList)
    self.shadowList = np.array(self.shadowList)

    lines = []
    fig = pyplot.figure()
    ax = fig.add_subplot(projection="3d")

    x = netFluxNorm[0]
    y = 0
    z = 0

    ax.quiver(0, 0, 0, x, y, z, colors='red')
    #lines.append(ax.plot3D(x,y,z, 'red', label='x Axis Local'))

    x = 0
    y = netFluxNorm[1]
    z = 0

    ax.quiver(0, 0, 0, x, y, z, colors='green')
    #lines.append(ax.plot3D(x,y,z, 'red', label='x Axis Local'))

    x = 0
    y = 0
    z = netFluxNorm[2]

    ax.quiver(0, 0, 0, x, y, z, colors='blue')
    #lines.append(ax.plot3D(x,y,z, 'red', label='x Axis Local'))

    x = -1 * netFluxNorm[3]
    y = 0
    z = 0

    ax.quiver(0, 0, 0, x, y, z, colors='red')
    #lines.append(ax.plot3D(x,y,z, 'red', label='x Axis Local'))

    

    x = 0
    y = -1 * netFluxNorm[4]
    z = 0

    ax.quiver(0, 0, 0, x, y, z, colors='green')
    #lines.append(ax.plot3D(x,y,z, 'red', label='x Axis Local'))

    x = 0
    y = 0
    z = -1 * netFluxNorm[5]

    ax.quiver(0, 0, 0, x, y, z, colors='blue')
    #lines.append(ax.plot3D(x,y,z, 'red', label='x Axis Local'))

    norm = np.max(np.linalg.norm(self.sunList, axis=1))
    list = self.sunList / norm

    x = list[:, 0, 0]
    y = list[:, 1, 0]
    z = list[:, 2, 0]

    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='yellow')
    lines.append(ax.plot3D(x,y,z, 'yellow', label='Sun'))

    norm = np.max(np.linalg.norm(self.ecefList, axis=1))
    list = self.ecefList / norm

    x = list[:, 0, 0]
    y = list[:, 1, 0]
    z = list[:, 2, 0]

    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='lime')
    lines.append(ax.plot3D(x,y,z, 'lime', label='Earth'))

    if self.shadowList.size > 1:
      norm = np.max(np.linalg.norm(self.shadowList, axis=1))
      list = self.shadowList / norm

      x = list[:, 0, 0]
      y = list[:, 1, 0]
      z = list[:, 2, 0]

      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='grey')
      lines.append(ax.plot3D(x,y,z, 'grey', label='Shadow'))

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

  def linePlots(self):
    time = np.array(self.timeList)
    fluxes = np.array(self.fluxList)

    _, subplots = pyplot.subplots(2, 1, sharex=True)
    subplots[-1].set_xlabel('Time (s)')

    px = fluxes[:, 0, 0]
    py = fluxes[:, 1, 0]
    pz = fluxes[:, 2, 0]
    nx = fluxes[:, 3, 0]
    ny = fluxes[:, 4, 0]
    nz = fluxes[:, 5, 0]
    subplots[0].plot(time, px, 'r')
    subplots[0].plot(time, py, 'g')
    subplots[0].plot(time, pz, 'b')
    subplots[0].plot(time, nx, 'r')
    subplots[0].plot(time, ny, 'g')
    subplots[0].plot(time, nz, 'b')
    subplots[0].set_title('Fluxes')
    subplots[0].axhline(y=0, color='k')
    subplots[0].set_ylabel('W/m^2')
    

def checkShadow(ecef, sunDir):
  #check if the satellite is in the earth's shadow
  x = np.linalg.norm(planeProjectNorm(sunDir,ecef))
  y = np.dot(ecef.flatten(), sunDir.flatten())
  flag = 1
  if x < EARTH_A and y < 0:
    flag = 0
  return flag

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

def findFluxes(faceNormals: np.ndarray, flux: np.ndarray):
  '''!@brief Calculates fluxes for each face based on angle
  to the flux vector

  @param faceNormals matrix of face normals
  @param flux current flux vector
  @return 6 element list of fluxes for each face
  '''
  faceFluxes = flux.flatten() @ faceNormals
  for j in range(0, 6):
    if faceFluxes[j] >= 0:
      faceFluxes[j] = 0 # in shadow
    else:
      faceFluxes[j] = -1*faceFluxes[j] # make positive

  return faceFluxes

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

# stolen from stackoverflow, slightly modified
# https://stackoverflow.com/questions/6451655/how-to-convert-python-datetime-dates-to-decimal-float-years
def toYearFraction(date):
    def sinceEpoch(date): # returns seconds since epoch
        return time.mktime(date.timetuple())
    s = sinceEpoch

    year = date.year
    startOfThisYear = dt.datetime(year=year, month=1, day=1)
    startOfNextYear = dt.datetime(year=year+1, month=1, day=1)

    yearElapsed = s(date) - s(startOfThisYear)
    yearDuration = s(startOfNextYear) - s(startOfThisYear)
    fraction = yearElapsed/yearDuration

    return fraction

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

def main():
  sim = Satellite(tMax, tStep, startTime, bbqOmega, maxSolarConst, minSolarConst, area, sunFace)
  sim.run()
  sim.plotLocal()
  sim.linePlots()
  pyplot.show()

main()
