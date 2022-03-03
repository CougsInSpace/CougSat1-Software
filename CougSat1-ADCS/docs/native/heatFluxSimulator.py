from pyorbital.orbital import Orbital
from pyorbital import astronomy
import datetime as dt
import numpy as np
import quaternion
import time
import math
import matplotlib.pyplot as pyplot

# Default starting point
startTime = dt.datetime.now()
tStep = 15 # tStep in seconds
bbqOmega = 0 # rad/s
maxSolarConst = 1322 # W/m^2
minSolarConst = 1414 # W/m^2
tMax = 90*60
area = .1*.1 #m^2
albedoFac = .55 # varies between .25 and .55 based on terrain
earthBBT = 255 # kelvin. Earth black body temp
boltzmann = 5.67e-8 # W/m^2K^4
emissivity = 1 # emissivity of satellite

# Constants
EARTH_A = 6378.137
EARTH_B = 6356.752

GEO_FACTOR = 1e-3
ALT_FACTOR = 1e4

# TLE for ISS
orb = Orbital(
  "ISS",
    line1="1 25544U 98067A   21249.54028389  .00002593  00000-0  55940-4 0  9993",
      line2="2 25544  51.6453 300.4471 0003214   1.7468 140.2698 15.48606005301202")
# updated
# orb = Orbital(
#   "ISS",
#   line1="25544U 98067A   22049.03783859  .00007772  00000-0  14488-3 0  9991",
#     line2="25544  51.6424 206.6447 0005686 140.2013 326.0656 15.49824186326703")

class Satellite:
  def __init__(self, Tf:float, tStep: float, startTime: dt.datetime, 
                bbqOmega: float, maxSolarConst: float, minSolarConst: float, area: float):
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
                                [0, 0,-1]]).T # add surface absorbtivity here (Local Frame)
    self.q = quaternion.from_float_array([1, 0, 0, 0])
    self.omegaq = quaternion.from_float_array(np.append([0], self.omega))
    self.qdot = .5*self.omegaq

    yearFraction = toYearFraction(self.currentTime)  
    offset = toYearFraction(dt.datetime(2000, 1, 4))
    self.currentFlux = math.sqrt(((minSolarConst*math.sin(math.pi*(yearFraction - offset)))**2) + ((maxSolarConst*math.cos(math.pi*(yearFraction - offset)))**2))

    self.latLong = np.array(orb.get_lonlatalt(self.currentTime))
    self.ecef = geo2ECEF(self.latLong[0], self.latLong[1], self.latLong[2])
    
    # lists for plotting
    self.fluxList = []
    self.netFlux = np.array([0,0,0,0,0,0])
    self.sunList = []
    self.ecefList = []
    self.shadowList = []
    self.timeList = []


  def run(self):
    n = int(np.ceil(self.tMax / self.tStep))
    for i in range(1,n):
      r = quaternion.as_rotation_matrix(self.q)
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
      IRFlux = IRFluxDir*boltzmann*emissivity*earthBBT
      faceIRFluxes = findFluxes(faceNormalsGlobal, IRFlux)

      # sum everything up
      totalFlux = faceSolarFluxes + faceAlbedofluxes + faceIRFluxes

      self.fluxList.append(totalFlux.reshape((6,1)))
      self.netFlux = self.netFlux + (totalFlux * tStep)
      self.sunList.append(sunDirection.reshape((3,1)))
      self.ecefList.append(self.ecef.reshape((3,1)))
      if not shadowBool:
        self.shadowList.append(self.ecef.reshape((3,1)))
      self.timeList.append(self.t)


      # update time
      self.currentTime = self.currentTime + dt.timedelta(seconds=self.tStep)
      self.t += self.tStep
      
      # update orientation and orbit
      self.q = self.q + (self.omegaq * self.tStep)
      self.latLong = np.array(orb.get_lonlatalt(self.currentTime))
      self.ecef = geo2ECEF(self.latLong[0], self.latLong[1], self.latLong[2])

      # calculate flux based on time of year
      yearFraction = toYearFraction(self.currentTime)  
      offset = toYearFraction(dt.datetime(2000, 1, 4))
      self.currentFlux = math.sqrt(((minSolarConst*math.sin(math.pi*(yearFraction - offset)))**2) + ((maxSolarConst*math.cos(math.pi*(yearFraction - offset)))**2))

    # self.plotLocal()
    time = np.array(self.timeList)
    fluxes = np.array(self.fluxList)
    for i in range(0, 1):
      list = np.concatenate(([time], [fluxes[:,i,0]]), axis=0).T
      directory = "Heat Flux Output"
      fileName = "flux-" + str(i) + ".xml"

      print(list)
      create_SW_XML(list, directory, fileName)

  def plotLocal(self):
    # plot everything
    self.fluxList = np.array(self.fluxList)
    netFluxNorm = self.netFlux / np.max(self.netFlux)
    self.sunList = np.array(self.sunList)

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

    pyplot.show()

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
  faceFluxes = flux.flatten() @ faceNormals
  for j in range(0, 6):
    if faceFluxes[j] >= 0:
      faceFluxes[j] = 0 # in shadow
    else:
      faceFluxes[j] = -1*faceFluxes[j]

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

def init_SW_XML(file_contents, path, name):
  #init_SW_XML(file_contents, path, name)#
  # Purpose: initializes a SW file that contains the data
  #
  # Variables: file_contents, type - string (contains contents of the file); 
  #             path, type - string (contains directory path to save the file); 
  #             name, type - string (name of the file including extention);
  #
  # Requirements: data in a necessary for SW_XML format, name variable has desired extention
  #
  # Returns: True (boolean), (success code)
  import os

  completeName = os.path.join(path, name)
  
  SW_file = open(completeName, "w")
  SW_file.write(file_contents)
  SW_file.close()

  return True # return success code

def parse_to_SW_format(data_stream):
  #parse_to_SW_format(data_stream)#
  # Purpose: transforms a numpy array into a desired format for SW XML file (string)
  #
  # Variables: data_stream, type - numpy array (2d array that contains datapoints of time (first entry), heat flux through each side (second entry)); 
  #
  # Requirements: data in a numpy array
  #
  # Returns: SW_string (str), (string in a format specific for SW XML file)
  SW_string = ""

  for data_point in data_stream:
      SW_string += "              <data points= \"" + str(data_point[0]) + " " + str(data_point[1]) + "\"/>\n"

  return SW_string

def create_SW_XML(data, path, name_w_extention):
  #create_SW_XML(data, path, name)#
  # Purpose: creates a SW file that contains the data in a necessary location
  #
  # Variables: data, type - numpy array (2d array that contains datapoints of time (first entry), heat flux through each side (second entry)); 
  #             path, type - string (contains directory path to save the file); 
  #             name, type - string (name of the file including extention);
  #
  # Returns: True (boolean), (success code of initialized file)
  header = "<?xml version=\"1.0\"?>\n\n<curves>\n          <functioncurve id=\"0\" name=\"Time curve\" type=\"1\" source=\"\" shape=\"0\">\"\n"

  bottom = "          </functioncurve>\n\n</curves>"

  parsed_SW_XML_data = parse_to_SW_format(data)

  file_contents = header + parsed_SW_XML_data + bottom

  init_SW_XML(file_contents, path, name_w_extention)

  return init_SW_XML(file_contents, path, name_w_extention)

def main():
  sim = Satellite(tMax, tStep, startTime, bbqOmega, maxSolarConst, minSolarConst, area)
  sim.run()

main()
