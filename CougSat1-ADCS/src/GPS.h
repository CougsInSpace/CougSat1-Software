/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file GPS.h
 * @author Cody Sigvartson
 * @date 21 October 2018
 * @brief Provides an interface for the on-board GPS 
 *
*/

#ifndef SRC_SYSTEMINTERFACES_GPS_H_
#define SRC_SYSTEMINTERFACES_GPS_H_

#include <mbed.h>
#include <DigitalIn.h>
#include <DigitalOut.h>
#include "ADCSPins.h"

// Error codes
#define GPS_NORMAL     0;
#define GPS_NACK       1;
#define GPS_TIMEOUT    2;
#define GPS_INVALIDARG 3;
#define GPS_UNKNOWN    4;

#define MSG_SIZE 120 // max size of NEMA response is 120 chars
#define GPS_ACK_TIMEOUT_MS 1000 // default wait time for how long the sender should wait for ack

class GPS {
  public:
    GPS(Serial &gps, DigitalIn &gpsResetPin, DigitalOut &gpsPulsePin, bool mode);
    ~GPS();
 
	// Accessors
	uint8_t getTemp() const;
	bool getMode() const;
	uint32_t getUtcTime() const;
	float getLat() const;
	float getLong() const;
	int32_t getAltitude() const;
	uint32_t getSpeedOverGround() const;
	uint8_t getDate() const;
	
	// Mutators
	void setMode(bool nMode);
	
	void read();
	
	// GPS configuration methods
    uint8_t setUpdateRate(uint8_t frequency, uint8_t attribute);
    uint8_t resetReceiver(bool reboot);
    uint8_t configNMEA(uint8_t messageName, bool enable, uint8_t attribute);
    uint8_t configNMEA(uint8_t nmeaByte, uint8_t attribute);
    uint8_t configPowerSave(bool enable, uint8_t attribute);
  private:
	// Serial interface (mbed.h)
	Serial &gps;
	
	// Digital pin interfaces (mbed.h)
	DigitalIn &resetPin;
	DigitalOut &pulsePin; // sends a 1hz pulse to adjust clock
	
	// GPS attributes
	uint8_t temp;
	bool mode;
	uint8_t nmeaState; // stores current configuration of which NMEA strings are enabled
	
	// RMC attributes
    uint32_t utcTime; // UTC time in hundredths of a second
	float lat;
	float longitude;
    int32_t altitude; // meters above sea level
    uint32_t speed; // speed over ground in knots
    uint8_t date; // date

	// Utility functions
	uint8_t rmcParser(uint8_t *nmea);
    uint8_t sendCommand(uint8_t messageid, uint8_t *messagebody, uint32_t bodylen); // uses default timeout specified by TIMEOUTMS
    uint8_t sendCommand(uint8_t messageid, uint8_t *messagebody, uint32_t bodylen, uint32_T timemout);
    uint8_t sendPacket(uint8_t *packet, uint32_t size, uint32_t timeout);
	void printPacket(uint8_t *packet, uint32_t size);
	// Initialize the GPS to default state
	void initialize();
};

#endif /* !SRC_SYSTEMINTERFACES_GPS_H_ */
