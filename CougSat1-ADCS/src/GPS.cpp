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
 * @date 21 October 2018
 * @brief Provides an interface to the on-board GPS 
 *
*/

#include "GPS.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

// TODO: implement
GPS::GPS(Serial &gps, DigitalIn &gpsResetPin, DigitalOut &gpsPulsePin, bool mode) : gps(gps), resetPin(gpsResetPin), pulsePin(gpsPulsePin){
	this->mode = mode;
	initialize();
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

uint32_t GPS::getUtcTime() const{
	return this->utcTime;
}

float GPS::getLat() const{
	return this->lat;
}

float GPS::getLong() const{
	return this->longitude;
}

int32_t GPS::getAltitude() const{
	return this->altitude;
}

uint32_t GPS::getSpeedOverGround() const{
	return this->speed;
}

uint8_t GPS::getDate() const{
	return this->date;
}

void GPS::setMode(bool nMode){
	if(this->mode != nMode){
		this->mode = nMode;
	}
}

// TODO: implement
void GPS::read(){
	// Read NMEA data from the GPS using mbed API
	// ie. gps.read(uint8_t *buffer, int length, const event_callback_t &callback, int event=SERIAL_EVENT_RX_COMPLETE, unsigned char char_match=SERIAL_RESERVED_CHAR_MATCH)
	// Parse data into usable format for subscribers of this data
}

// Attribute:
// 0 = only update RAM
// 1 = update RAM and flash
uint8_t GPS::setUpdateRate(uint8_t frequency, uint8_t attribute)
{
    DEBUG("GPS","Setting update rate\n");
    uint8_t messageBody[2];
    memset(messageBody, 0, 2);
    messageBody[0] = frequency;
    messageBody[1] = attribute;
    return sendCommand(0x0E, messageBody, 2);
}
}

uint8_t GPS::resetReceiver(bool reboot)
{
    DEBUG("GPS","Resetting receiver\n");
    uint8_t messageBody[1];
    memset(messageBody, 0, 1);
    messageBody[0] = reboot ? 1 : 0;
    uint8_t code = sendCommand(0x04, messageBody, 1, 10000);
    if (code == GPS_NORMAL)
    {
        delay(500);
		// TODO: restart the gps
        gps.end();
        gps.begin(GPS_DEFAULT_BAUDRATE);
    }
    return code;
}

// Attribute:
// 0 = only update RAM
// 1 = update RAM and flash
uint8_t GPS::configNMEA(char messageName, bool enable, uint8_t attribute)
{
    DEBUG("GPS","Configuring a NMEA string\n");
    if (enable)
        nmeaState |= 1 << messageName;
    else
        nmeaState &= ~(1 << messageName);
    return configNMEA(nmeaState, attribute);
}

// Attribute:
// 0 = only update RAM
// 1 = update RAM and flash
// 2 = temporarily enabled
char GPS::configPowerSave(bool enable, uint8_t attribute)
{
    DEBUG("GPS","Configuring Power Save mode\n");
    char messageBody[2];
    memset(messageBody, 0, 2);
    messageBody[0] = enable ? 1 : 0;
    messageBody[1] = attribute;
    return sendCommand(0x0C, messageBody, 2);
}

// Attribute:
// 0 = only update RAM
// 1 = update RAM and flash
char GPS::configNMEA(char nmeaByte, uint8_t attribute)
{
    DEBUG("GPS","Configuring all NMEA strings\n");
    nmeaState = nmeaByte;
    char messageBody[8];
    memset(messageBody, 0, 8);
	// determine which nmea sentences are enabled/disabled
	messageBody[0] = (nmeaState >> NMEA_GGA) & 1;
    messageBody[1] = (nmeaState >> NMEA_GSA) & 1;
    messageBody[2] = (nmeaState >> NMEA_GSV) & 1;
    messageBody[3] = (nmeaState >> NMEA_GLL) & 1;
    messageBody[4] = (nmeaState >> NMEA_RMC) & 1;
    messageBody[5] = (nmeaState >> NMEA_VTG) & 1;
    messageBody[6] = (nmeaState >> NMEA_ZDA) & 1;
    messageBody[7] = attribute;
    return sendCommand(0x08, messageBody, 8);
}

// Private Utility Functions for parsing NMEA data
/*
	Time, date, position, course and speed data provided by a GNSS navigation receiver.
	Structure:
	$GPRMC,hhmmss.sss,A,dddmm.mmmm,a,dddmm.mmmm,a,x.x,x.x,ddmmyy,,,a*hh<CR><LF>
	Example:
	$GPRMC,111636.932,A,2447.0949,N,12100.5223,E,000.0,000.0,030407,,,A*61<CR><LF>
*/
uint8_t GPS::rmcParser(uint8_t *nmea){
	/*
	TODO: implement
	Parse the RMC NMEA sentence and populate:
	1. UTC time
	2. latitude
	3. longitude
	4. altitude
	5. speed over ground
	6. date
	*/
}

uint8_t GPS::sendCommand(uint8_t messageId, uint8_t *messageBody, uint32_t bodyLen)
{
    return sendCommand(messageId, messageBody, bodyLen, GPS_ACK_TIMEOUT_MS);
}

uint8_t GPS::sendCommand(uint8_t messageId, uint8_t *messageBody, uint32_t bodyLen, uint32_t timeout)
{
    DEBUG("GPS","sending command\n");
    // Assemble Packet
    uint32_t packetLength = 8 + bodyLen;
    uint8_t packet[packetLength];
    memset(packet, 0, packetLength);

    packet[0] = 0xA0; // start sequence
    packet[1] = 0xA1;

    packet[2] = (uint8_t) ((bodyLen + 1) >> 8); // payload length includes message id
    packet[3] = (uint8_t) bodyLen + 1;

    packet[4] = messageId;

    // calculate checksum
    uint8_t checksum = messageId;
    for (int i = 5; i < packetLength - 3; i++)
    {
        packet[i] = messageBody[i - 5];
        checksum ^= packet[i];
    }
    packet[packetLength - 3] = checksum;

    packet[packetLength - 2] = 0x0D; // terminate command with crlf
    packet[packetLength - 1] = 0x0A;

    // Send Packet
    printPacket(packet, packetLength);

    uint8_t code = sendPacket(packet, packetLength, timeout / 2);
    DEBUG("GPS","response code ");
    DEBUG("GPS",code);

    if (code != GPS_NORMAL)
    {
        DEBUG("GPS","failed, trying again\n");
        code = sendPacket(packet, packetLength, timeout / 2);
        DEBUG("GPS","response code ");
        DEBUG("GPS",code);
    }
    return code;
}

uint8_t GPS::sendPacket(uint8_t *packet, int32_t size, uint32_t timeout)
{
    uint8_t c = 0;
    uint8_t last = 0;
    bool response = false;
    gps.write(packet, size);
    // TODO: wait for repsonse, need to use different API to get current
	// time in ms
    for(uint32_t start = millis(); millis() - start < timeout;)
    {
        while (gps.available())
        {
            c = gps.read();
            if (last == 0xA0 && c == 0xA1 && response == false)
                response = true;
            if (response && last == 0x83)
            {
                if (c == packet[4]) // packet[4] = messageid
                    return GPS_NORMAL;
                else
                    return GPS_UNKNOWN;
            }
            else if (response && last == 0x84)
            {
                if (c == packet[4]) // packet[4] = messageid
                    return GPS_NACK;
                else
                    return GPS_UNKNOWN;
            }
            last = c;
        }
    }
    return GPS_TIMEOUT;
}

void GPS::printPacket(uint8_t *packet, uint32_t size)
{
    DEBUG("GPS","assembled Packet: {");
    for (int i = 0; i < size; i++)
    {
        char hexval[4];
        sprintf(hexval, "0x%02X", packet[i]);
        DEBUG("GPS",hexval);
        if (i < size - 1) {DEBUG("GPS",", ");}
    }
    DEBUG("GPS","}");
}

// TODO: implement
void GPS::initialize(){
	// This function should initialize the GPS into a "default" state
	// Use the sendCommand() function to configure the GPS
}

// END GPS_CPP IMPLEMENTATION