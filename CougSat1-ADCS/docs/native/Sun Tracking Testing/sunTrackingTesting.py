import pyorbital
from pyorbital import astronomy
import matplotlib.pyplot as plt
import numpy as np
import datetime as dt

def main():
    currenttime = dt.datetime(2022,9,20)
    timeDelta = dt.timedelta(hours=1)
    raList=[]
    decList = []
    for i in range(24*3):
        raDec = astronomy.sun_ra_dec(currenttime)
        newlong = astronomy.gmst(currenttime) - raDec[0]
        if newlong > np.pi:
            newlong -= 2*np.pi
        raList.append(newlong)
        decList.append(raDec[1])
        currenttime += timeDelta

    x = np.array(range(24*3))
    y = np.array(raList)

    plt.plot(x,np.rad2deg(y))
    plt.show()

    #gmst = greenwich mean sidereal time , which should be used with right ascension to determine longitude
    #gmst is measured in radians
    #autumnal equinox is when right ascension resets, passing over the astronomical version of the international date line

    #print(currenttime)
    #print(astronomy.gmst(currenttime))
    #print(np.rad2deg(astronomy.gmst(currenttime)))
if __name__ == "__main__":
    main()