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

#include "GPS.h"

// TODO: implement
GPS::GPS(bool mode){
	this->mode = mode;
}

// TODO: implement
GPS::~GPS(){
	// free up any dynamic memory allocation
}

GPS::uint8 getTemp() const{
	return this->temp;
}

GPS::bool getMode() const{
	return this->mode;
}

void GPS::setMode(bool nMode){
	if(this->mode != nMode){
		this->mode = nMode;
	}
}

// TODO: implement
bool GPS::beginConnection(void){
	// Open a connection with the GPS using mbed Serial object
	// pins shall be defined in ADCSpins.h
	// ie. Serial gps(GPS_PIN1, GPS_PIN2);
}

// TODO: implement
bool GPS::closeConnection(void){
	// If there is a current connection, close it
}

// TODO: implement
void GPS::read(){
	// Read NMEA data from the GPS using mbed API
	// ie. gps.read(uint8_t *buffer, int length, const event_callback_t &callback, int event=SERIAL_EVENT_RX_COMPLETE, unsigned char char_match=SERIAL_RESERVED_CHAR_MATCH)
	// Parse data into usable format for subscribers of this data
}

// Private Utility Functions for parsing NMEA data
// TODO: implement
GPSFixData GPS::nmeaToGPSFixData(unsigned char *nmea){
	// Parse NMEA GPSFixData into usable format for subcribers of this data
	// NEMA message format: https://github.com/CougsInSpace/Resources/blob/master/SupplierDocuments/SkyTraq/Venus838FLPx-SPC_CubeSatGPS.pdf
}

// TODO: implement
GPSLatLong GPS::nmeaToLatLong(unsigned char *nmea){
	// Parse NMEA GPSLatLong (GLL) into usable format for subcribers of this data
	// NEMA message format: https://github.com/CougsInSpace/Resources/blob/master/SupplierDocuments/SkyTraq/Venus838FLPx-SPC_CubeSatGPS.pdf
}

// TODO: implement
GPSDOP GPS::nmeaToGDOP(unsigned char *nmea){
	// Parse NMEA GPSGDOP into usable format for subcribers of this data
	// NEMA message format: https://github.com/CougsInSpace/Resources/blob/master/SupplierDocuments/SkyTraq/Venus838FLPx-SPC_CubeSatGPS.pdf
}

// TODO: implement
GPSGSV GPS::nmeaToGPGSV(unsigned char *nmea){
	// Parse NMEA GPSGPGSV into usable format for subcribers of this data
	// NEMA message format: https://github.com/CougsInSpace/Resources/blob/master/SupplierDocuments/SkyTraq/Venus838FLPx-SPC_CubeSatGPS.pdf	
}

// TODO: implement
GPSRMC GPS::nmeaToRMC(unsigned char *nmea){
	// Parse NMEA GPSRMC into usable format for subcribers of this data
	// NEMA message format: https://github.com/CougsInSpace/Resources/blob/master/SupplierDocuments/SkyTraq/Venus838FLPx-SPC_CubeSatGPS.pdf	
}

// TODO: implement
GPSVTG GPS::nmeaToVTG(unsigned char *nmea){
	// Parse NMEA GPSVTG into usable format for subcribers of this data
	// NEMA message format: https://github.com/CougsInSpace/Resources/blob/master/SupplierDocuments/SkyTraq/Venus838FLPx-SPC_CubeSatGPS.pdf
}

// TODO: implement
GPSZDA GPS::nmeaToZDA(unsigned char *nmea){
	// Parse NMEA GPSZDA into usable format for subcribers of this data
	// NEMA message format: https://github.com/CougsInSpace/Resources/blob/master/SupplierDocuments/SkyTraq/Venus838FLPx-SPC_CubeSatGPS.pdf	
}

// END GPS_CPP IMPLEMENTATION