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
 * @date 22 October 2018
 * @brief Provides an interface to the on-board GPS
 *
 */

#include "Venus838FLPx.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

// TODO: implement
Venus838FLPx::Venus838FLPx(Serial & serial, DigitalOut & reset, DigitalIn & pulse, bool mode)
  : serial(serial), reset(reset), pulse(pulse) {
  this->nmeaState = 0x0; // disable all nmea messages
}

// TODO: implement
Venus838FLPx::~Venus838FLPx() {
  // free up any dynamic memory allocation
}
/*
bool GPS::getMode() const{
        return this->mode;
}
*/
uint32_t Venus838FLPx::getUtcTime() const {
  return this->rmcData.utcDate;
}

float Venus838FLPx::getLat() const {
  return this->rmcData.latitude;
}

float Venus838FLPx::getLong() const {
  return this->rmcData.longitude;
}

//Altitude is not given in RMC data
/*
int32_t Venus838FLPx::getAltitude() const {
  return this->altitude;
}*/

uint32_t Venus838FLPx::getSpeedOverGround() const {
  return this->rmcData.speedOverGround;
}

uint8_t Venus838FLPx::getDate() const {
  return this->rmcData.utcDate;
}

// TODO: implement
uint8_t Venus838FLPx::read() {
  char data [83] = {'\0'};
  serial.gets(data, 83);
  
  //TODO: setup different data parsers for other formats
  rmcParser(data);
}

// Attribute:
// RAM_ONLY = only update RAM
// RAM_FLASH = update RAM and flash
uint8_t Venus838FLPx::setUpdateRate(uint8_t frequency, Attributes_t attribute) {
  DEBUG("GPS", "Setting update rate\n");
  uint8_t messageBody[2];
  memset(messageBody, 0, 2);
  messageBody[0] = frequency;
  messageBody[1] = attribute;
  return sendCommand(0x0E, messageBody, 2);
}

uint8_t Venus838FLPx::resetReceiver(bool reboot) {
  DEBUG("GPS", "Resetting receiver\n");
  uint8_t messageBody[1];
  memset(messageBody, 0, 1);
  messageBody[0] = reboot ? 1 : 0;
  uint8_t code   = sendCommand(0x04, messageBody, 1, 10000);
  if (code == ERROR_SUCCESS) {
    // delay(500);
    ThisThread::sleep_for(500);
    // TODO: restart the gps
  } else {
    DEBUG("GPS", "Unable to reset receiver\n")
  }
  return code;
}

// Attribute:
// RAM_ONLY = only update RAM
// RAM_FLASH = update RAM and flash
uint8_t Venus838FLPx::configNMEA(
    uint8_t messageName, bool enable, Attributes_t attribute) {
  DEBUG("GPS", "Configuring a NMEA string\n");
  if (enable)
    nmeaState |= 1 << messageName;
  else
    nmeaState &= ~(1 << messageName);
  return configNMEA(nmeaState, attribute);
}

// Attribute:
// RAM_ONLY = only update RAM
// RAM_FLASH = update RAM and flash
uint8_t Venus838FLPx::configNMEA(uint8_t nmeaByte, Attributes_t attribute) {
  DEBUG("GPS", "Configuring all NMEA strings\n");
  nmeaState = nmeaByte;
  uint8_t messageBody[8];
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

// Attribute:
// RAM_ONLY = only update RAM
// RAM_FLASH = update RAM and flash
// TEMP = temporarily enabled
uint8_t Venus838FLPx::configPowerSave(bool enable, Attributes_t attribute) {
  DEBUG("GPS", "Configuring Power Save mode\n");
  uint8_t messageBody[2];
  memset(messageBody, 0, 2);
  messageBody[0] = enable ? 1 : 0;
  messageBody[1] = attribute;
  return sendCommand(0x0C, messageBody, 2);
}

void Venus838FLPx::setMode(bool nMode) {
  // this will ultimately call sendCommand() to wake/sleep the GPS
}

// TODO: implement
uint8_t Venus838FLPx::initialize() {
 uint8_t messageBody[2];
 char response[21];

  configNMEA(0x10,RAM_FLASH); //disable all NMEA mesages exept RMC and set that to default on device
  messageBody[0] = 0;
  sendCommandResponce(0x2,messageBody,1,response,21);
  DEBUG("GPS", "Software Kernal Version: %d.%d.%d \n Software ODM Version: %d.%d.%d \n Revision Date (YMD): %d/%d/%d\n ")
        response[7], response[8],response[9],response[11],response[12],response[13], response[14],response[15],response[16];
  
  sendCommandResponce(0x3,messageBody,1,response,11);
  DEBUG("GPS", "Software CRC Version: %d%d") response[7],response[8];
}

uint8_t Venus838FLPx::rmcParser(char * nmea) {

  uint8_t error = ERROR_SUCCESS;

  strtok(nmea, ",");//remove device id from mesage
  if(*strtok(NULL,",") == 'A'){
  rmcData.utcTime = atof(strtok(NULL, ","));//UTC time field
  rmcData.latitude = atof(strtok(NULL,","));//lattituded field
  if(*strtok(NULL,",") == 'S'){ // if in souther hemishphere save as negative value
    rmcData.latitude *= -1;
  }
  rmcData.longitude = atof(strtok(NULL,",")); //longitude field
  if(*strtok(NULL,",") == 'E'){ // if in eastern hemishphere save as negative value
    rmcData.longitude *= -1;
  }
  rmcData.speedOverGround = atof(strtok(NULL,",")); // speed over ground in knots
  rmcData.courseOverGround = atof(strtok(NULL,",")); //course over ground in degrees
  rmcData.utcDate = atoi(strtok(NULL,","));
  }else{
    error = ERROR_INVALID_DATA;
  }
  return error;
}

uint8_t Venus838FLPx::sendCommand(uint8_t messageId, uint8_t * messageBody,
    uint32_t bodyLen, uint32_t timeout) {
  DEBUG("GPS", "sending command\n");
  // Assemble Packet
  uint32_t packetLength = 8 + bodyLen;
  char  packet[packetLength];
  memset(packet, 0, packetLength);

  packet[0] = 0xA0; // start sequence
  packet[1] = 0xA1;

  packet[2] =
      (uint8_t)((bodyLen + 1) >> 8); // payload length includes message id
  packet[3] = (uint8_t)bodyLen + 1;

  packet[4] = messageId;

  // calculate checksum
  // We should implement a general CougSat error detection class
  // to perform checksums, CRCs, etc
  uint8_t checksum = messageId;
  for (int i = 5; i < packetLength - 3; i++) {
    packet[i] = messageBody[i - 5];
    checksum ^= packet[i];
  }
  packet[packetLength - 3] = checksum;

  packet[packetLength - 2] = 0x0D; // terminate command with crlf
  packet[packetLength - 1] = 0x0A;

  // Send Packet
  printPacket(packet, packetLength);

  char code = sendPacket(packet, packetLength, timeout / 2);
  DEBUG("GPS", "response code ");
  DEBUG("GPS", &code);

  if (code != ERROR_SUCCESS) {
    DEBUG("GPS", "failed, trying again\n");
    code = sendPacket(packet, packetLength, timeout / 2);
    DEBUG("GPS", "response code ");
    DEBUG("GPS", &code);
  }
  return code;
}

uint8_t Venus838FLPx::sendCommandResponce(uint8_t messageid, uint8_t * messagebody,
      uint32_t bodylen, char *response, uint8_t responseLen, uint32_t timeout){

      uint8_t code = sendCommand(messageid,messagebody,bodylen,timeout);
      serial.gets(response,responseLen); // add the seven extra values of package
      return code;

      }

uint8_t Venus838FLPx::sendPacket(char * packet, uint32_t size, uint32_t timeout) {
  uint8_t c        = 0;
  uint8_t last     = 0;
  bool    response = false;
  serial.puts(packet);
  // TODO: wait for ACK, need to use different API to get current
  // time in ms
  Timer t;
  for (t.start(); t.read_ms() < timeout;) {
    while (serial.readable()) {
      c = serial.getc();
      if (last == 0xA0 && c == 0xA1 && response == false)
        response = true;
      if (response && last == ACK) {
        if (c == packet[4]) // packet[4] = messageid
          return ERROR_SUCCESS;
        else
          return ERROR_UNKNOWN_COMMAND;
      } else if (response && last == NACK) {
        if (c == packet[4]) // packet[4] = messageid
          return ERROR_NACK;
        else
          return ERROR_UNKNOWN_COMMAND;
      }
      last = c;
    }
  }
  return ERROR_WAIT_TIMEOUT;
}

void Venus838FLPx::printPacket(char * packet, uint32_t size) {
  DEBUG("GPS", "assembled Packet: {");
  for (int i = 0; i < size; i++) {
    char hexval[4];
    sprintf(hexval, "0x%02X", packet[i]);
    DEBUG("GPS", hexval);
    if (i < size - 1) {
      DEBUG("GPS", ", ");
    }
  }
  DEBUG("GPS", "}");
}
// END GPS_CPP IMPLEMENTATION