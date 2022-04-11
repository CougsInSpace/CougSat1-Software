from pyorbital.orbital import Orbital
from pyorbital import astronomy
import datetime as dt
import numpy as np
import quaternion
import time
import math
import matplotlib.pyplot as pyplot
from solid_works_curves import *


class Orbit:
    def __init__(
        self,
        TLE: Orbital,
    ):
        self.TLE = TLE


class Satellite:
    def __init__(
        self,
        sunFace: np.ndarray,
        Sattelite_Orbit: Orbit,
        area: float = 0.0,
        bbqOmega: float = 0.0,
    ):
        self.orbit = Sattelite_Orbit
        self.area = area
        self.sunFace = sunFace
        self.omega = np.array([0, 0, bbqOmega])  # (Global Frame)
        self.area = area
        self.faceNormals = np.array(
            [[1, 0, 0], [0, 1, 0], [0, 0, 1], [-1, 0, 0], [0, -1, 0], [0, 0, -1]]
        ).T  # TODO add surface absorbtivity here (Local Frame)
        self.q_orientation = None
        self.qdot = None

    def find_q_orientation(
        self,
        currentTime: dt.datetime,
    ):
        if np.linalg.norm(sunFace) != 0:
            sunDirection = sunDir(currentTime)
            axis = np.cross(sunFace.flatten(), sunDirection.flatten())
            angle = find_angle(sunFace, sunDirection)
            axAng = (axis / np.linalg.norm(axis)) * angle
            self.q_orientation = quaternion.from_rotation_vector(axAng)
        else:
            self.q_orientation = quaternion.from_float_array([1, 0, 0, 0])

        return self.q_orientation

    def find_q_dot(self):

        omegaq = quaternion.from_float_array(np.append([0], self.omega))
        self.qdot = 0.5 * omegaq * self.q_orientation

        return self.qdot

    def simulate_heat_flux(
        self,
        startDate: dt.datetime,
        tStep: int,
        TotalSimTime: int,
        maxSolarConst: float = 1414.0,  # W/m^2
        minSolarConst: float = 1322.0,  # W/m^2
    ):
        currentDate = startDate  # getting current date
        offset = toYearFraction(dt.datetime(2022, 1, 4))  # getting offset time

        self.q_orientation = self.find_q_orientation(currentDate)
        self.qdot = self.find_q_dot()

        # output variables
        fluxList = []
        net_flux = np.zeros(6)
        in_shadowList = []
        shadowList = []  # list of coordinates when in shadow
        sunList = []
        ecefList = []
        timeList = []

        # main for loop
        for sim_time in range(0, TotalSimTime + tStep, tStep):

            # get position of satellite in orbit
            ecef, _ = self.orbit.TLE.get_position(currentDate, normalize=False)
            ecef = np.array(ecef).reshape((3, 1))

            yearFraction = toYearFraction(currentDate)

            currentFlux = math.sqrt(
                ((minSolarConst * math.sin(math.pi * (yearFraction - offset))) ** 2)
                + ((maxSolarConst * math.cos(math.pi * (yearFraction - offset))) ** 2)
            )

            r = quaternion.as_rotation_matrix(
                self.find_q_orientation(currentDate)
            )  # local to global
            rInv = np.linalg.inv(r)  # global to local
            faceNormalsGlobal = r @ self.faceNormals

            # find sun direction and check if the satellite is in shadow
            sunDirection = sunDir(currentDate)
            in_shadow = is_inShadow(ecef, sunDirection)

            # solar flux
            solarFluxDir = -1 * sunDirection
            solarFlux = solarFluxDir * currentFlux
            faceSolarFluxes = findFluxes(faceNormalsGlobal, solarFlux)
            faceSolarFluxes = faceSolarFluxes * in_shadow

            # albedo flux
            albedoFluxDir = ecef / np.linalg.norm(ecef)
            albedoFlux = albedoFluxDir * currentFlux
            faceAlbedofluxes = findFluxes(faceNormalsGlobal, albedoFlux)
            faceAlbedofluxes = faceAlbedofluxes * in_shadow

            # ir flux
            IRFluxDir = albedoFluxDir
            IRFlux = IRFluxDir * boltzmann * emissivity * (earthBBT**4)
            faceIRFluxes = findFluxes(faceNormalsGlobal, IRFlux)

            # sum everything up
            totalFlux = faceSolarFluxes + faceAlbedofluxes + faceIRFluxes

            fluxList.append(totalFlux.reshape((6, 1)))
            sunList.append(rInv @ sunDirection.reshape((3, 1)))
            ecefList.append(rInv @ ecef.reshape((3, 1)))
            net_flux = net_flux + totalFlux * tStep
            timeList.append(sim_time)
            in_shadowList.append(in_shadow)

            if not in_shadow:
                shadowList.append(rInv @ ecef.reshape((3, 1)))

            # update orientation and orbit
            self.q_orientation = self.q_orientation + (self.qdot * tStep)

            self.qdot = (
                0.5
                * quaternion.from_float_array(np.append([0], self.omega))
                * self.qdot
            )
            self.q_orientation = self.q_orientation / abs(self.q_orientation)

            # update date
            currentDate += dt.timedelta(seconds=tStep)

        return [
            fluxList,
            timeList,
            net_flux,
            in_shadowList,
            shadowList,
            sunList,
            ecefList,
        ]


def is_inShadow(ecef, sunDir):
    # check if the satellite is in the earth's shadow
    x = np.linalg.norm(planeProjectNorm(sunDir, ecef))
    y = np.dot(ecef.flatten(), sunDir.flatten())

    return not (x < EARTH_A and y < 0)


def planeProjectNorm(n, u):
    """!@brief Projects vector onto using plane normal as plane
    definition

    @param n plane normal vector
    @param u vector to project
    @return u projected onto plane defined by n
    """
    n = n.flatten()
    u = u.flatten()

    normal = vectorProject(u, n)

    return u - normal


def vectorProject(u, v):
    """!@brief Projects u onto v


    @param u vector to project
    @param v vector to be projected onto
    @return vector projection of u onto v
    """
    return (np.dot(v, u) / (np.linalg.norm(v) ** 2)) * v


def findFluxes(faceNormals: np.ndarray, flux: np.ndarray):
    """!@brief Calculates fluxes for each face based on angle
    to the flux vector

    @param faceNormals matrix of face normals
    @param flux current flux vector
    @return 6 element list of fluxes for each face
    """
    faceFluxes = flux.flatten() @ faceNormals
    for j in range(0, 6):
        if faceFluxes[j] >= 0:
            faceFluxes[j] = 0  # in shadow
        else:
            faceFluxes[j] = -1 * faceFluxes[j]  # make positive

    return faceFluxes


def sunDir(time: dt.datetime):
    """!@brief Find position of sun in Global frame

    @param time Current time
    @return sunDir Direction vector of sun in Global frame
    """
    ascDec = astronomy.sun_ra_dec(time)
    long = np.degrees(ascDec[0])
    lat = np.degrees(ascDec[1])

    sunDir = geo2ECEF(long, lat, 1)
    sunDir = sunDir / np.linalg.norm(sunDir.flatten())

    return sunDir.reshape((3, 1))


def toYearFraction(
    date,
):  # stolen from stackoverflow, slightly modified,   https://stackoverflow.com/questions/6451655/how-to-convert-python-datetime-dates-to-decimal-float-years
    def sinceEpoch(date):  # returns seconds since epoch
        return time.mktime(date.timetuple())

    s = sinceEpoch

    year = date.year
    startOfThisYear = dt.datetime(year=year, month=1, day=1)
    startOfNextYear = dt.datetime(year=year + 1, month=1, day=1)

    yearElapsed = s(date) - s(startOfThisYear)
    yearDuration = s(startOfNextYear) - s(startOfThisYear)
    fraction = yearElapsed / yearDuration

    return fraction


def geo2ECEF(longitude: float, latitude: float, altitude: float) -> np.ndarray:
    """!@brief Convert geodetic coordinates to Earth-Centered, Earth-Fixed coordinates (XYZ)

    @param longitude Longitude in degrees
    @param latitude Latitude in degrees
    @param altitude Height above sea level in km
    @return np.ndarray shape=(3, 1) XYZ coordinates in km
    """

    latitude = np.deg2rad(latitude)
    longitude = np.deg2rad(longitude)

    N = EARTH_A**2 / np.sqrt(
        EARTH_A**2 * np.cos(latitude) ** 2 + EARTH_B**2 * np.sin(latitude) ** 2
    )

    nCosLat = (N + altitude) * np.cos(latitude)
    x = nCosLat * np.cos(longitude)
    y = nCosLat * np.sin(longitude)
    z = (EARTH_B**2 / EARTH_A**2 * N + altitude) * np.sin(latitude)
    return np.array([[x], [y], [z]])


def find_angle(v1, v2):
    v1 = v1.flatten()
    v2 = v2.flatten()
    # Find the unit vectors first
    v1unit = v1 / np.linalg.norm(v1)
    v2unit = v2 / np.linalg.norm(v2)
    # Find the dot product between v1 and v2 unit vectors
    dotProduct = np.dot(v1unit, v2unit)

    # if statement to handle rounding errors that make
    # dotProduct > 1 when v1=v2
    if dotProduct < 1:
        # Take arccos to find the angle in radians between the two
        anglebetween = np.arccos(dotProduct)
    else:
        anglebetween = 0

    return anglebetween


def linePlots(timeList: list, fluxList: list):

    time = np.array(timeList)
    fluxes = np.array(fluxList)

    _, subplots = pyplot.subplots(2, 1, sharex=True)
    subplots[-1].set_xlabel("Time (s)")

    px = fluxes[:, 0, 0]
    py = fluxes[:, 1, 0]
    pz = fluxes[:, 2, 0]
    nx = fluxes[:, 3, 0]
    ny = fluxes[:, 4, 0]
    nz = fluxes[:, 5, 0]
    subplots[0].plot(time, px, "r")
    subplots[0].plot(time, py, "g")
    subplots[0].plot(time, pz, "b")
    subplots[0].plot(time, nx, "r")
    subplots[0].plot(time, ny, "g")
    subplots[0].plot(time, nz, "b")
    subplots[0].set_title("Fluxes")
    subplots[0].axhline(y=0, color="k")
    subplots[0].set_ylabel("W/m^2")

    pyplot.show()


"""Constants"""
EARTH_A = 6378.137  # big radius of earth (km)
EARTH_B = 6356.752  # small radius of earth (km)
EARTH_MEAN = (EARTH_A + EARTH_B) / 2

GEO_FACTOR = 1e-3
ALT_FACTOR = 1e4


maxSolarConst = 1322  # W/m^2
minSolarConst = 1414  # W/m^2
albedoFac = 0.55  # earth albedo factor, varies between .25 and .55 based on terrain
earthBBT = 255  # kelvin. Earth black body temp
boltzmann = 5.67e-8  # W/m^2K^4
emissivity = 1  # emissivity of satellite (IR absorptivity)


# TLE for ISS
ISS_orb = Orbital(
    "ISS",
    line1="1 25544U 98067A   21249.54028389  .00002593  00000-0  55940-4 0  9993",
    line2="2 25544  51.6453 300.4471 0003214   1.7468 140.2698 15.48606005301202",
)
"""Constants END"""


"""INITIALIZATION PARAMETERS START"""
# Simulation parameters
tStep = 60  # tStep in seconds
tMax = 270 * 60  # time simulation runs for in seconds


# Satellite parameters
bbqOmega = 0  # rad/s WARNING: higher than .005 will neccesitate a smaller tstep
startTime = dt.datetime(2023, 7, 9)


sunFace = np.array(
    [1, 0, 0]
)  # direction vector for direction on satellite pointing at sun

area = 0.1 * 0.1  # m^2

orb_TLE = ISS_orb  # TLE for ISS

"""INITIALIZATION PARAMETERS END"""


fileNames = ["+x", "+y", "+z", "-x", "-y", "-z"]


CubeSat_Orbit = Orbit(orb_TLE)
CubeSat = Satellite(sunFace, CubeSat_Orbit, area)

output = CubeSat.simulate_heat_flux(startTime, tStep, tMax)

fluxList, timeList = output[0], output[1]

linePlots(timeList, fluxList)
