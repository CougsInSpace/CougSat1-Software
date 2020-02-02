#include "CDH.h"

#include <CISConsole.h>

/**
 * @brief Construct a new CDH::CDH object
 *
 * @param i2c connected to the C&DH
 */
CDH::CDH(I2CSlave & i2c) : i2c(i2c) {}

/**
 * @brief Checks if a message is available to read
 *
 * @return true is the PMIC is addressed
 */
bool CDH::hasMessage() {
  int state = i2c.receive();
  return state == I2CSlave::ReadAddressed || state == I2CSlave::WriteAddressed;
}

/**
 * @brief Process the message coming from the C&DH
 *
 * @return mbed_error_code_t error code
 */
mbed_error_code_t CDH::processMessage() {
  // Get message
  // Switch on commandID and fire off the proper function
  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Process the power change message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_code_t CDH::processMsgPowerChange(char * msgBody) {
  // Do what it says
  // Give them what they want
  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Process the voltage request message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_code_t CDH::processMsgVoltageRequest(char * msgBody) {
  // Do what it says
  // Give them what they want
  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Process the current request message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_code_t CDH::processMsgCurrentRequest(char * msgBody) {
  // Do what it says
  // Give them what they want
  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Process the temperature request message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_code_t CDH::processMsgTemperatureRequest(char * msgBody) {
  // Do what it says
  // Give them what they want
  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Process the power channel status message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_code_t CDH::processMsgPowerChannelStatus(char * msgBody) {
  // Do what it says
  // Give them what they want
  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Process the solar panel channel status message
 *
 * @param msgBody array to the incoming message body
 * @return mbed_error_code_t error code
 */
mbed_error_code_t CDH::processMsgSolarChannelStatus(char * msgBody) {
  // Do what it says
  // Give them what they want
  return MBED_ERROR_UNSUPPORTED;
}