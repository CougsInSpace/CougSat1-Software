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
}

/**
 * @brief Construct a new LTC2499::LTC2499 object
 *
 * @param i2c connected to the ADC
 * @param addr of the AC
 * @param refVoltage at the REF+ pin (VRef / 2)
 */
LTC2499::LTC2499(I2C &i2c, uint8_t addr, float refVoltage) : i2c(i2c) {
  this->addr = addr;
  this->refVoltage = refVoltage;
  setVRef(refVoltage);
}

/**
 * @brief Reads the voltage of a channel
 *
 * @param channel to measure
 * @param data to store voltage value
 * @return error code
 */
uint8_t LTC2499::readVoltage(ADCChannel_t channel, float *data) {
  int32_t buf;
  uint8_t result = readRaw(channel, &buf);
  (*data) = (float)buf * conversionFactor;
  return result;
}

/**
 * @brief Reads the internal temperature sensor
 *
 * @param data to store the temperature (celsius)
 * @return error code
 */
uint8_t LTC2499::readInternalTemperaure(float *data) {
  int32_t buf;
  uint8_t result = readRaw(INT_TEMP, ADC_CONFIG_TEMP_50_60_1x, &buf);
  (*data) = (float)buf * refVoltage / ADC_TEMP_SLOPE + ADC_ZERO_KELVIN;
  return result;
}

/**
 * @brief Calibrates the conversion factor based on the VRef
 *
 * @param refVoltage at the REF+ pin (VRef / 2)
 */
void LTC2499::setVRef(float refVoltage) {
  this->refVoltage = refVoltage;
  conversionFactor = refVoltage / (float)ADC_FULL_SCALE;
}

/**
 * @brief Calibrates the conversion factor based on the expected
 *  voltage at a channel
 *
 * @param refVoltage at the REF+ pin (VRef / 2)
 * @param channel to calibrate to
 * @return error code
 */
uint8_t LTC2499::setVRef(float refVoltage, ADCChannel_t channel) {
  int32_t rawVRef;
  uint8_t result = readRaw(channel, &rawVRef);
  if (result != ERROR_SUCCESS) {
    DEBUG("LTC2499", "Error setting vRef from channel %d", channel);
    return result;
  }
  conversionFactor = refVoltage / rawVRef;
  return ERROR_SUCCESS;
}

/**
 * @brief Reads the ADC value of a channel
 *
 * @param channel to measure
 * @param data to store the value
 * @return error code
 */
uint8_t LTC2499::readRaw(ADCChannel_t channel, int32_t *data) {
  return readRaw(channel, ADC_CONFIG_EXT_50_60_1x, data);
}

/**
 * @brief Reads the ADC value of a channel
 *
 * @param channel to measure
 * @param config of the ADC conversion
 * @param data to store the value
 * @return error code
 */
uint8_t LTC2499::readRaw(ADCChannel_t channel, uint8_t config, int32_t *data) {
  char buf[4] = {channel, config, 0, 0};
  uint8_t result = i2c.write(addr, buf, 2);
  if (result != ERROR_SUCCESS) {
    DEBUG("LTC2499", "Error commanding channel %d", channel);
    return ERROR_NACK;
  }

  result = i2c.read(addr, buf, 4);
  if (result != ERROR_SUCCESS) {
    DEBUG("LTC2499", "Error requesting channel %d", channel);
    return ERROR_NACK;
  }

  uint8_t prefixBits = (buf[3] >> 6) & 0x03;

  if (prefixBits == 0x03) {
    DEBUG("LTC2499", "Channel %d is overrange", channel);
    (*data) = ADC_OVERRANGE;
  } else if (prefixBits == 0x00) {
    DEBUG("LTC2499", "Channel %d is underrange", channel);
    (*data) = ADC_UNDERRANGE;
  } else {
    int32_t rawValue = (buf[3] << 18);
    rawValue |= (buf[2] << 10);
    rawValue |= (buf[1] << 2);
    rawValue |= (buf[0] >> 6);
    (*data) = rawValue & ADC_RAW_MASK;
  }

  return ERROR_SUCCESS;
}