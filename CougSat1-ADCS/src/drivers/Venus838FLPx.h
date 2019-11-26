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
 * @date 22 October 2018
 * @brief Provides an interface for the on-board GPS
 *
 */

#ifndef SRC_SYSTEMINTERFACES_GPS_H_
#define SRC_SYSTEMINTERFACES_GPS_H_

#include "ADCSPins.h"
#include <DigitalIn.h>
#include <DigitalOut.h>
#include <chrono>
#include <mbed.h>
#include <rtos.h>

#define MSG_SIZE 120 // max size of NEMA response is 120 chars
#define GPS_ACK_TIMEOUT_MS                                                     \
  1000 // default wait time for how long the sender should wait for ack

#define NMEA_GGA 0
#define NMEA_GSA 1
#define NMEA_GSV 2
#define NMEA_GLL 3
#define NMEA_RMC 4
#define NMEA_VTG 5
#define NMEA_ZDA 6

#define NMEA_MAX_LENGTH 83

#define ACK 0x83  // message id of ACK
#define NACK 0x84 // message id of NACK

// TODO: test the underlying types of enum values, typed enum
// support in C++11
typedef enum attributes : uint8_t {
  RAM_ONLY  = 0,
  RAM_FLASH = 1,
  TEMP      = 2
} Attributes_t;

typedef struct rmc {
  float   utcTime;
  float   latitude;
  float   longitude;
  float   speedOverGround;  // measures sspeed in knots
  float   courseOverGround; // measures speed in degrees
  uint8_t utcDate;
} RMC;

class Venus838FLPx {
public:
  Venus838FLPx(
      Serial & serial, DigitalOut & reset, DigitalIn & pulse, bool mode);
  ~Venus838FLPx();

  // Accessors
  // bool getMode() const;
  uint32_t getUtcTime() const;
  float    getLat() const;
  float    getLong() const;
  int32_t  getAltitude() const;
  uint32_t getSpeedOverGround() const;
  uint8_t  getDate() const;

  // Mutators
  void setMode(bool nMode);

  uint8_t read();

  uint8_t setUpdateRate(uint8_t frequency, Attributes_t attribute);
  uint8_t resetReceiver(bool reboot);
  uint8_t configNMEA(uint8_t messageName, bool enable, Attributes_t attribute);
  uint8_t configNMEA(uint8_t nmeaByte, Attributes_t attribute);
  uint8_t configPowerSave(bool enable, Attributes_t attribute);

  // Initialize the GPS to default state
  uint8_t initialize();

private:
  // Serial interface (mbed.h)
  Serial & serial;

  // Digital pin interfaces (mbed.h)
  DigitalOut & reset;
  DigitalIn &  pulse; // sends a 1hz pulse to adjust clock

  // GPS attributes
  uint8_t nmeaState; // stores current configuration of which NMEA strings are
                     // enabled

  RMC rmcData;

  // Utility functions
  uint8_t rmcParser(char * nmea);
  uint8_t sendCommand(uint8_t messageid, uint8_t * messagebody,
      uint32_t bodylen, uint32_t timeout = GPS_ACK_TIMEOUT_MS);
  uint8_t sendCommandResponce(uint8_t messageid, uint8_t * messagebody,
      uint32_t bodylen, char * response,
      uint32_t timeout =
          GPS_ACK_TIMEOUT_MS); // overloaded function to indirecly return
                               // response to the command
  uint8_t sendPacket(char * packet, uint32_t size, uint32_t timeout);
  void    printPacket(char * packet, uint32_t size);
};

#endif /* !SRC_SYSTEMINTERFACES_GPS_H_ */
