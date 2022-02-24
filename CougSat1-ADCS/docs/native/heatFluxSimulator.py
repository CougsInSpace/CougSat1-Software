from pyorbital.orbital import Orbital
from pyorbital import astronomy
import datetime as dt
import numpy as np
import quaternion
import time
import math

# Default starting point
startTime = dt.datetime.now()
tStep = 15*60 # tStep in seconds
initialOmega = np.array([0,0,.1]) # rad/s
maxSolarConst = 1322 # W/m^2
minSolarConst = 1414 # W/m^2
tMax = 90*60*60
area = .1*.1 #m^2

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
                initialOmega: np.ndarray, maxSolarConst: float, minSolarConst: float, area: float):
    self.currentTime = startTime
    self.t = 0
    self.tMax = Tf
    self.omega = initialOmega
    self.tStep = tStep
    self.area = area
    self.faceNormals = np.array([[1, 0, 0],
                                [0, 1, 0],
                                [0, 0, 1],
                                [-1,0, 0],
                                [0,-1, 0],
                                [0, 0,-1]]).T
    self.fluxList = np.ndarray
    # set up quatenion orientation

    yearFraction = toYearFraction(self.currentTime)  
    offset = toYearFraction(dt.datetime(2000, 1, 4))
    self.currentFlux = math.sqrt(((minSolarConst*math.sin(math.pi*(yearFraction - offset)))**2) + ((maxSolarConst*math.cos(math.pi*(yearFraction - offset)))**2))

    self.latLong = np.array(orb.get_lonlatalt(self.currentTime))
    self.ecef = geo2ECEF(self.latLong[0], self.latLong[1], self.latLong[2])

  def run(self):
    n = int(np.ceil(self.tMax / self.tStep))
    for i in range(1,n):
      sunDirection = sunDir(self.currentTime)

      solarFluxDir = -1*sunDirection
      solarFlux = solarFluxDir*self.currentFlux
      faceSolarFluxes = findFluxes(self.faceNormals, solarFlux)
      faceSolarFluxes = faceSolarFluxes * checkShadow(self.ecef, sunDirection)

      # update class parameters
      # add time


      
      

def checkShadow(ecef, sunDir):
  x = np.linalg.norm(planeProjectNorm(sunDir,ecef))
  flag = 1
  if x < EARTH_A:
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

def main():
  sim = Satellite(tMax, tStep, startTime, initialOmega, maxSolarConst, minSolarConst, area)
  sim.run()

main()
