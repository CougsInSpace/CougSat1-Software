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
#include "tools/CISError.h"
#include "tools/CISConsole.h"

/**
 * @brief Construct a new LTC2499::LTC2499 object
 *
 * @param i2c connected to the ADC
 * @param addr of the ADC
 */
LTC2499::LTC2499(I2C &i2c, uint8_t addr) : i2c(i2c) {
  this->addr = addr;
  conversionFactor = 0.0f;
  refVoltage = 0.0f;
  configuredChannel = DIFF_0;
}

/**
 * @brief Construct a new LTC2499::LTC2499 object
 *
 * @param i2c connected to the ADC
 * @param addr of the AC
 * @param refVoltage at the REF+ pin (VRef / 2)
 */
LTC2499::LTC2499(I2C &i2c, uint8_t addr, double refVoltage, double gain) : i2c(i2c) {
  this->addr = addr;
  this->refVoltage = refVoltage;
  setVRef(refVoltage, gain);
}

/**
 * @brief Reads the voltage of last configured channel
 *
 * @param data to store voltage value
 * @return error code
 */
uint8_t LTC2499::readVoltage(double *data) {
  int32_t buf;
  uint8_t result = readRaw(&buf);
  (*data) = (double)buf * conversionFactor;
  return result;
}

/**
 * @brief Reads the voltage of a channel
 *
 * @param channel to measure
 * @param data to store voltage value
 * @return error code
 */
uint8_t LTC2499::readVoltage(ADCChannel_t channel, double *data) {
  int32_t buf;
  uint8_t result = selectChannel(channel);
  if (result != ERROR_SUCCESS) {
    DEBUG("LTC2499", "Error changing to temperature channel");
    return result;
  }
  result = readRaw(&buf);
  if (result != ERROR_SUCCESS) {
    DEBUG("LTC2499", "Error reading temperature channel");
    return result;
  }
  (*data) = (double)buf * conversionFactor;
  return result;
}

/**
 * @brief Reads the internal temperature sensor
 *
 * @param data to store the temperature (celsius)
 * @return error code
 */
uint8_t LTC2499::readInternalTemperaure(double *data) {
  int32_t buf;
  uint8_t result = configureChannel(INT_TEMP, ADC_CONFIG_TEMP_50_60_1x);
  if (result != ERROR_SUCCESS) {
    DEBUG("LTC2499", "Error changing to temperature channel");
    return result;
  }
  result = readRaw(&buf);
  if (result != ERROR_SUCCESS) {
    DEBUG("LTC2499", "Error reading temperature channel");
    return result;
  }
  (*data) = (double)buf * refVoltage / ADC_TEMP_SLOPE + ADC_ZERO_KELVIN;
  return result;
}

/**
 * @brief Selects the next channel to read
 * Waits up to 200ms for configure to happen if blocking
 *
 * @param channel to select
 * @param blocking or not
 * @return error code
 */
uint8_t LTC2499::selectChannel(ADCChannel_t channel, bool blocking) {
  uint8_t result = 0;
  uint8_t conversionTimeout = ADC_CONVERSION_TIMEOUT;
  // Typical 1x conversions will take definitely less than 200ms
  // LTC2499 cannot be communicated to during conversion process
  // LTC2499 returns NACK during conversion process
  // Wait 5ms at a time up to 200ms
  do {
    result = configureChannel(channel, ADC_CONFIG_EXT_50_60_1x);
    if (result != ERROR_SUCCESS) {
      if (conversionTimeout <= 0) {
        DEBUG("LTC2499", "Error selecting channel %d", configuredChannel);
        return ERROR_NACK;
      } else {
        wait_ms(5);
        conversionTimeout -= 5;
      }
    } else {
      blocking = false;
    }
  } while(blocking);
  return result;
}

/**
 * @brief Configures the next channel to be read
 *
 * @param channel to configure
 * @return error code
 */
uint8_t LTC2499::configureChannel(ADCChannel_t channel, uint8_t config) {
  char buf[2] = {(char)(channel | ADC_CONFIG_CHANNEL), (char)config};
  DEBUG("LTC2499", "Writing 0x%02X%02X to 0x%02X", buf[0], buf[1], addr);
  uint8_t result = i2c.write(addr, buf, 2);
  if (result != ERROR_SUCCESS) {
    DEBUG("LTC2499", "Error configuring channel %d", channel);
    return ERROR_NACK;
  }
  configuredChannel = channel;
  return ERROR_SUCCESS;
}

/**
 * @brief Calibrates the conversion factor based on the VRef
 *
 * @param refVoltage at the REF+ pin (VRef / 2)
 * @param gain of the ADC inputs
 */
void LTC2499::setVRef(double refVoltage, double gain) {
  this->refVoltage = refVoltage;
  conversionFactor = refVoltage / (double)ADC_FULL_SCALE * gain;
}

/**
 * @brief Calibrates the conversion factor based on the expected
 *  voltage at a channel. The exernal gain is automatically compensated
 *
 * @param refVoltage at the REF+ pin (VRef / 2)
 * @param channel to calibrate to
 * @return error code
 */
uint8_t LTC2499::setVRef(double refVoltage, ADCChannel_t channel) {
  int32_t rawVRef;
  uint8_t result = selectChannel(channel);
  if (result != ERROR_SUCCESS) {
    DEBUG("LTC2499", "Error changing to vRef channel %d", channel);
    return result;
  }
  result = readRaw(&rawVRef);
  if (result != ERROR_SUCCESS) {
    DEBUG("LTC2499", "Error reading vRef from channel %d", channel);
    return result;
  }
  conversionFactor = refVoltage / rawVRef;
  return ERROR_SUCCESS;
}

/**
 * @brief Reads the ADC value of last configured channel
 * Waits up to 200ms for conversion to happen if blocking
 *
 * @param data to store the value
 * @param blocking or not
 * @return error code
 */
uint8_t LTC2499::readRaw(int32_t *data, bool blocking) {
  char buf[4] = {0, 0, 0, 0};
  uint8_t result = 0;
  // Typical 1x conversions will take definitely less than 200ms
  // LTC2499 returns NACK during conversion process
  // Wait 5ms at a time up to 200ms
  uint8_t conversionTimeout = ADC_CONVERSION_TIMEOUT;
  do {
    result = i2c.read(addr, buf, 4);
    if (result != ERROR_SUCCESS) {
      if (conversionTimeout <= 0) {
        DEBUG("LTC2499", "Error requesting channel %d", configuredChannel);
        return ERROR_NACK;
      } else {
        wait_ms(5);
        conversionTimeout -= 5;
      }
    } else {
      blocking = false;
    }
  } while(blocking);

  DEBUG("LTC2499", "Raw 0x%02X%02X%02X%02X", buf[0], buf[1], buf[2], buf[3]);

  uint8_t prefixBits = (buf[0] >> 6) & 0x03;
  int32_t rawValue = (buf[0] << 18);
  rawValue |= (buf[1] << 10);
  rawValue |= (buf[2] << 2);
  rawValue |= (buf[3] >> 6);
  DEBUG("LTC2499", "prefixBits: 0x%02X", prefixBits);
  switch (prefixBits) {
  case 0x00:
    DEBUG("LTC2499", "Channel %d is underrange", configuredChannel);
    (*data) = ADC_UNDERRANGE;
    break;
  case 0x01:
    // Negative value
    (*data) = rawValue | ADC_RAW_MASK_NEG;
    DEBUG("LTC2499", "Read 0x%08X = %d", (*data), (*data));
    break;
  case 0x02:
    // Positive value
    (*data) = rawValue & ADC_RAW_MASK_POS;
    DEBUG("LTC2499", "Read 0x%08X = %d", (*data), (*data));
    break;
  case 0x03:
    DEBUG("LTC2499", "Channel %d is overrange", configuredChannel);
    (*data) = ADC_OVERRANGE;
    break;
  }

  return ERROR_SUCCESS;
}
