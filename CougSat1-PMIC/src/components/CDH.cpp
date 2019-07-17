/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file CDH.cpp
 * @author Bradley Davis
 * @date 7 Nov 2018
 * @brief Interfacing class to the C&DH subsystem
 *
 */

#include "CDH.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

/**
 * @brief Construct a new CDH::CDH object
 *
 * @param i2c connected to the C&DH
 */
CDH::CDH(I2C & i2c) : i2c(i2c) {}

/**
 * @brief Handles the message coming from the C&DH
 *
 * @return uint8_t error code
 */
uint8_t CDH::processMessage() {
  // Tell C&DH ready to recieve
  // Get message
  // Switch on commandID and fire off the proper function
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Handles the power change message
 *
 * @param msgBody array to the incoming message body
 * @return uint8_t error code
 */
uint8_t CDH::processMsgPowerChange(char * msgBody) {
  // Do what it says
  // Give them what they want
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Handles the voltage request message
 *
 * @param msgBody array to the incoming message body
 * @return uint8_t error code
 */
uint8_t CDH::processMsgVoltageRequest(char * msgBody) {
  // Do what it says
  // Give them what they want
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Handles the current request message
 *
 * @param msgBody array to the incoming message body
 * @return uint8_t error code
 */
uint8_t CDH::processMsgCurrentRequest(char * msgBody) {
  // Do what it says
  // Give them what they want
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Handles the temperature request message
 *
 * @param msgBody array to the incoming message body
 * @return uint8_t error code
 */
uint8_t CDH::processMsgTemperatureRequest(char * msgBody) {
  // Do what it says
  // Give them what they want
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Handles the power channel status message
 *
 * @param msgBody array to the incoming message body
 * @return uint8_t error code
 */
uint8_t CDH::processMsgPowerChannelStatus(char * msgBody) {
  // Do what it says
  // Give them what they want
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Handles the solar panel channel status message
 *
 * @param msgBody array to the incoming message body
 * @return uint8_t error code
 */
uint8_t CDH::processMsgSolarChannelStatus(char * msgBody) {
  // Do what it says
  // Give them what they want
  return ERROR_NOT_SUPPORTED;
}