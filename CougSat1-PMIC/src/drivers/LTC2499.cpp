/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file LTC2499.cpp
 * @author Bradley Davis
 * @date 13 Apr 2018
 * @brief Driver to talk to a LTC2499 ADC
 *
 */
#include "LTC2499.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"
#include <math.h>

/**
 * @brief Construct a new LTC2499::LTC2499 object
 *
 * @param i2c connected to the LTC2499
 * @param addr of the LTC2499
 */
LTC2499::LTC2499(I2C & i2c, uint8_t addr) : i2c(i2c) {
  this->addr           = addr;
  vRefVoltage          = 0.0;
  currentActiveChannel = 0;
  configuredChannel    = DIFF_0;
  for (int i = 0; i < 16; i++) {
    voltagesSingle[i] = NAN;
  }
  for (int i = 0; i < 8; i++) {
    voltagesDiff[i] = NAN;
  }
}

/**
 * @brief Construct a new LTC2499::LTC2499 object
 *
 * @param i2c connected to the LTC2499
 * @param addr of the LTC2499
 * @param refVoltage at the REF+ pin
 * @param gain of the ADC input
 */
LTC2499::LTC2499(I2C & i2c, uint8_t addr, double refVoltage, double gain) :
  i2c(i2c) {
  this->addr  = addr;
  vRefVoltage = refVoltage / 2.0;
  setVRef(refVoltage, gain);
  currentActiveChannel = 0;
  configuredChannel    = DIFF_0;
  for (int i = 0; i < 16; i++) {
    voltagesSingle[i] = NAN;
  }
  for (int i = 0; i < 8; i++) {
    voltagesDiff[i] = NAN;
  }
}

/**
 * @brief Reads the voltage of last configured channel
 *
 * @param data to store voltage value (NULL okay)
 * @param blocking or not
 * @return error code
 */
uint8_t LTC2499::readVoltage(double * data, bool blocking) {
  int32_t buf;
  uint8_t result  = readRaw(&buf);
  double  voltage = (double)buf * conversionFactor;

  if (data != NULL) {
    (*data) = voltage;
  }
  LOG("LTC2499", "Channel 0x%02X on 0x%02X is %7.4fV", configuredChannel, addr,
      voltage);

  setVoltage(configuredChannel, voltage);
  return result;
}

/**
 * @brief Reads the voltage of a channel
 *
 * @param channel to measure
 * @param data to store voltage value
 * @param blocking or not
 * @return error code
 */
uint8_t LTC2499::readVoltage(
    LTC2499::ADCChannel_t channel, double * data, bool blocking) {
  uint8_t result = selectChannel(channel);
  if (result != ERROR_SUCCESS) {
    ERROR(
        "LTC2499", "Error changing to channel 0x%02X on 0x%02X", channel, addr);
    return result;
  }

  int32_t buf;
  result = readRaw(&buf, blocking);
  if (result != ERROR_SUCCESS) {
    ERROR("LTC2499", "Error reading channel 0x%02X from 0x%02X", channel, addr);
    return result;
  }

  (*data) = (double)buf * conversionFactor;
  ERROR(
      "LTC2499", "Channel 0x%02X on 0x%02X is %7.4fV", channel, addr, (*data));
  return result;
}

/**
 * @brief Reads the current channel and selects the next channel
 * The ADC will send the previous channel's conversion result and perform a
 * conversion on the newly selected channel
 *
 * Datasheet page 22: Continuous read/write
 *
 * @param data to read into
 * @param nextChannel to select
 * @param blocking or not
 * @return uint8_t error code
 */
uint8_t LTC2499::readVoltageSelectNext(
    double * data, LTC2499::ADCChannel_t nextChannel, bool blocking) {
  // Configure channel with repeated start condition
  uint8_t result = selectChannel(nextChannel, blocking, true);
  if (result != ERROR_SUCCESS) {
    ERROR("LTC2499", "Error changing to channel 0x%02X with repeated start",
        nextChannel);
    return result;
  }

  // Read the voltage in the ADC's buffer
  result = readVoltage(data, blocking);

  // Update the configured channel
  configuredChannel = nextChannel;
  return result;
}

/**
 * @brief Reads the next channel in the active channel queue
 * Gets the value for the configuredChannel
 * Changes channel to the next in the queue
 *
 * @param blocking or not
 * @return error code
 */
uint8_t LTC2499::readNextActiveChannel(bool blocking) {
  if (activeChannels.empty()) {
    return ERROR_INVALID_DATA;
  }
  currentActiveChannel = (currentActiveChannel + 1) % activeChannels.size();
  return readVoltageSelectNext(
      NULL, activeChannels.at(currentActiveChannel), blocking);
}

/**
 * @brief Adds the channel to the active channel queue
 * Set the appropriate bit in activeChannels
 *
 * @param channel to add
 */
void LTC2499::addActiveChannel(LTC2499::ADCChannel_t channel) {
  activeChannels.push_back(channel);
}

/**
 * @brief Gets the last read voltage for the channel
 *
 * @param channel to get
 * @return voltage
 */
double LTC2499::getVoltage(LTC2499::ADCChannel_t channel) {
  if ((channel & 0x10) != 0) {
    return voltagesSingle[channel & 0x0F];
  }
  double sign = ((channel & 0x08) == 0) ? 1.0 : -1.0;
  return sign * voltagesDiff[channel & 0x07];
}

/**
 * @brief Set the voltage for the channel
 *
 * @param channel to set
 * @param voltage
 */
void LTC2499::setVoltage(LTC2499::ADCChannel_t channel, double voltage) {
  if ((channel & 0x10) != 0) {
    voltagesSingle[channel & 0x0F] = voltage;
  } else {
    double sign = ((channel & 0x08) == 0) ? 1.0 : -1.0;

    voltagesDiff[channel & 0x07] = sign * voltage;
  }
}

/**
 * @brief Reads the internal temperature sensor
 *
 * @param data to store the temperature
 * @param blocking or not
 * @return error code
 */
uint8_t LTC2499::readInternalTemperaure(double * data, bool blocking) {
  if (data == NULL) {
    ERROR("LTC2499", "Temperature data is NULL");
    return ERROR_INVALID_ARGS;
  }

  uint8_t result = configureChannel(INT_TEMP, LTC2499_CONFIG_TEMP_50_60_1x);
  if (result != ERROR_SUCCESS) {
    ERROR("LTC2499", "Error changing to temperature channel on 0x%02X", addr);
    return result;
  }

  int32_t buf;
  result = readRaw(&buf, blocking);
  if (result != ERROR_SUCCESS) {
    ERROR("LTC2499", "Error reading temperature channel from 0x%02X", addr);
    return result;
  }

  (*data) = (double)buf * vRefVoltage / LTC2499_TEMP_SLOPE;
  return result;
}

/**
 * @brief Selects the next channel to read
 * Waits up to 200ms for configure to happen if blocking
 *
 * @param channel to select
 * @param blocking or not
 * @param repeated start or not
 * @return error code
 */
uint8_t LTC2499::selectChannel(
    LTC2499::ADCChannel_t channel, bool blocking, bool repeated) {
  uint8_t result =
      configureChannel(channel, LTC2499_CONFIG_EXT_60_1x, blocking, repeated);
  if (result != ERROR_SUCCESS) {
    ERROR("LTC2499", "Error selecting channel 0x%02X on 0x%02X", channel, addr);
    return result;
  }
  return result;
}

/**
 * @brief Configures the next channel to be read
 *
 * @param channel to configure
 * @param configuration of channel
 * @param repeated start or not
 * @return error code
 */
uint8_t LTC2499::configureChannel(LTC2499::ADCChannel_t channel, uint8_t config,
    bool blocking, bool repeated) {
  char buf[2] = {(char)(channel | LTC2499_CONFIG_CHANNEL), (char)config};
  DEBUG("LTC2499", "Writing 0x%02X%02X to 0x%02X", buf[0], buf[1], addr);

  // Typical 1x conversions will take definitely less than 200ms
  // LTC2499 cannot be communicated to during conversion process
  // LTC2499 returns NACK during conversion process
  // Wait 5ms at a time up to 200ms
  uint8_t conversionTimeout = LTC2499_CONVERSION_TIMEOUT;
  uint8_t result            = ERROR_SUCCESS;
  do {
    result = i2c.write(addr, buf, 2);
    if (result != ERROR_SUCCESS) {
      if (conversionTimeout <= 0 || !blocking) {
        ERROR("LTC2499", "Error configuring channel 0x%02X on 0x%02X", channel,
            addr);
        if (repeated) {
          // i2c.stop();
        }
        return ERROR_NACK;
      } else {
        if (repeated) {
          // i2c.stop();
        }
        wait_ms(5);
        conversionTimeout -= 5;
      }
    } else {
      blocking = false;
    }
  } while (blocking);

  if (!repeated) {
    configuredChannel = channel;
  }
  return ERROR_SUCCESS;
}

/**
 * @brief Calibrates the conversion factor based on the VRef
 *
 * @param refVoltage at the REF+ pin
 * @param gain of the LTC2499 inputs
 */
void LTC2499::setVRef(double refVoltage, double gain) {
  vRefVoltage      = refVoltage / 2.0;
  conversionFactor = vRefVoltage / (double)LTC2499_FULL_SCALE * gain;
}

/**
 * @brief Calibrates the conversion factor based on the expected
 *  voltage at a channel. The exernal gain is automatically compensated
 *
 * @param refVoltage at reference channel
 * @param gain of the LTC2499 inputs
 * @param channel to calibrate to
 * @return error code
 */
uint8_t LTC2499::setVRef(
    double refVoltage, double gain, LTC2499::ADCChannel_t channel) {
  uint8_t result = selectChannel(channel);
  if (result != ERROR_SUCCESS) {
    ERROR("LTC2499", "Error changing to vRef channel 0x%02X on 0x%02X", channel,
        addr);
    return result;
  }

  int32_t rawVRef;
  result = readRaw(&rawVRef);
  if (result != ERROR_SUCCESS) {
    ERROR("LTC2499", "Error reading vRef from channel 0x%02X on 0x%02X",
        channel, addr);
    return result;
  }

  conversionFactor = refVoltage / rawVRef;
  vRefVoltage      = conversionFactor * (double)LTC2499_FULL_SCALE * gain;
  return ERROR_SUCCESS;
}

/**
 * @brief Returns the voltage at the Ref pin
 *
 * @return voltage
 */
double LTC2499::getVRef() {
  return (vRefVoltage * 2.0);
}

/**
 * @brief Reads the LTC2499 value of last configured channel
 * Waits up to 200ms for conversion to happen if blocking
 *
 * @param data to store the value
 * @param blocking or not
 * @return error code
 */
uint8_t LTC2499::readRaw(int32_t * data, bool blocking) {
  if (data == NULL) {
    ERROR("LTC2499", "Read raw data is NULL");
    return ERROR_INVALID_ARGS;
  }

  uint8_t result = 0;

  char buf[4] = {0, 0, 0, 0};

  // Typical 1x conversions will take definitely less than 200ms
  // LTC2499 returns NACK during conversion process
  // Wait 5ms at a time up to 200ms
  uint8_t conversionTimeout = LTC2499_CONVERSION_TIMEOUT;
  do {
    result = i2c.read(addr, buf, 4);
    if (result != ERROR_SUCCESS) {
      if (conversionTimeout <= 0) {
        ERROR("LTC2499", "Error requesting channel 0x%02X from 0x%02X",
            configuredChannel, addr);
        return ERROR_NACK;
      } else {
        wait_ms(5);
        conversionTimeout -= 5;
      }
    } else {
      blocking = false;
    }
  } while (blocking);

  DEBUG("LTC2499", "Raw 0x%02X%02X%02X%02X from 0x%02X", buf[0], buf[1], buf[2],
      buf[3], addr);

  uint8_t prefixBits = (buf[0] >> 6) & 0x03;
  DEBUG("LTC2499", "prefixBits: 0x%02X from 0x%02X", prefixBits, addr);

  int32_t rawValue = (buf[0] << 18);
  rawValue |= (buf[1] << 10);
  rawValue |= (buf[2] << 2);
  rawValue |= (buf[3] >> 6);

  switch (prefixBits) {
    case 0x00:
      DEBUG("LTC2499", "Channel 0x%02X is underrange on 0x%02X",
          configuredChannel, addr);
      (*data) = LTC2499_UNDERRANGE;
      break;
    case 0x01:
      // Negative value
      (*data) = rawValue | LTC2499_RAW_MASK_NEG;
      DEBUG("LTC2499", "Read 0x%08X = %d from 0x%02X", (*data), (*data), addr);
      break;
    case 0x02:
      // Positive value
      (*data) = rawValue & LTC2499_RAW_MASK_POS;
      DEBUG("LTC2499", "Read 0x%08X = %d from 0x%02X", (*data), (*data), addr);
      break;
    case 0x03:
      DEBUG("LTC2499", "Channel 0x%02X is overrange on 0x%02X",
          configuredChannel, addr);
      (*data) = LTC2499_OVERRANGE;
      break;
  }

  return ERROR_SUCCESS;
}
