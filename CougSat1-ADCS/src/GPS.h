/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file GPS.cpp
 * @author Cody Sigvartson
 * @date 7 October 2018
 * @brief Controls all intracommunication on the satellite
 *
*/

#ifndef SRC_SYSTEMINTERFACES_GPS_H_
#define SRC_SYSTEMINTERFACES_GPS_H_

#typedef unsigned char uint8

#include <mbed.h>
#include <rtos.h>
#include <filesystem/fat/FATFileSystem.h>
#include "drivers/SDBlockDevice.h"
#include "ADCSPins.h"

// TODO: define this
typedef struct GPSFixData{
	
}GPSFixData;

// TODO: define this
typedef struct GPSLatLong{
	
}GPSLatLong;

// TODO: define this
typedef struct GPSDOP{
	
}GPSDOP;

// TODO: define this
typedef struct GPSGSV{
	
}GPSGSV;

// TODO: define this
typedef struct GPSRMC{
	
}GPSRMC;

// TODO: define this
typedef struct GPSVTG{
	
}GPSVTG;

// TODO: define this
typedef struct GPSZDA{
	
}GPSZDA;

class GPS {
  public:
    GPS(bool mode);
    ~GPS();
 
	uint8 getTemp() const;
	bool getMode() const;
	void setMode(bool nMode);
	bool beginConnection(void);
	bool closeConnection(void);
	void read();
  private:
    //System interfaces
    ADCS adcs;
	IHU ihu;
	
    //Hardware drivers
    SDBlockDevice sd;
    FATFileSystem fs;
	
	// Serial interface (mbed.h)
	Serial gps;
	
	// GPS attributes
	uint8 temp;
	bool mode;
	
	// Utility functions
	GPSFixData nmeaToGPSFixData(char *nmew);
	GPSLatLong nmeaToLatLong(char *nmea);
	GPSDOP nmeaToGDOP(char *nmea);
	GPSGSV nmeaToGPGSV(char *nmea);
	GPSRMC nmeaToRMC(char *nmea);
	GPSVTG nmeaToVTG(char *nmea);
	GPSZDA nmeaToZDA(char *nmea);
};

#endif /* !SRC_SYSTEMINTERFACES_GPS_H_ */
