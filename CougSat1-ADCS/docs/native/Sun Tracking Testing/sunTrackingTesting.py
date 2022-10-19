from cmath import pi
import pyorbital
from pyorbital import astronomy
import matplotlib.pyplot as plt
import numpy as np
import datetime as dt

def main():
    # CURRENT TIME MUST BE IN UTC TIME ZONE 
    # currenttime = dt.datetime(2023,3,20, hour=12,minute=9,second=4) # default minute 9
    currenttime = dt.datetime(2020,8,20, hour=0)
    # currenttime = dt.datetime.now()#dt.datetime(2020,8,20, hour=3)
    
    ra, lat = astronomy.sun_ra_dec(currenttime)
    long = ra - astronomy.gmst(currenttime)
    long = long % (2*pi)
    if long > pi:
        long = (-2*pi) + long
    elif long < -pi:
        long = 2*pi + long
    print(currenttime)
    print(astronomy.gmst(currenttime))
    # print(np.rad2deg(long))
    # print(np.rad2deg(lat))
    # print(currenttime)
    # timeDelta = dt.timedelta(hours=23, minutes=56)
    # raList=[]
    # decList = []
    # for i in range(24*3):
    #     raDec = astronomy.sun_ra_dec(currenttime)
    #     newlong = astronomy.gmst(currenttime) + raDec[0]
    #     if newlong > np.pi:
    #         newlong -= 2*np.pi
    #     raList.append(newlong)
    #     decList.append(raDec[1])
    #     currenttime += timeDelta

    # x = np.array(range(24*3))
    # y = np.array(raList)

    # plt.plot(x,np.rad2deg(y))
    # plt.show()
    # print(astronomy.gmst(currenttime + timeDelta))
    #gmst = greenwich mean sidereal time , which should be used with right ascension to determine longitude
    #gmst is measured in radians
    #autumnal equinox is when right ascension resets, passing over the astronomical version of the international date line

    #print(currenttime)
    #print(astronomy.gmst(currenttime))
    #print(np.rad2deg(astronomy.gmst(currenttime)))

if __name__ == "__main__":
    main()