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

LTC2499::LTC2499(I2C &i2c, uint8_t addr) : i2c(i2c) { this->addr = addr; }

float LTC2499::readVoltage(ADCChannel_t channel) {
  int32_t rawValue = readRaw(channel);
  return rawValue * conversionFactor;
}

uint8_t LTC2499::setVRef(float refVoltage) {
  conversionFactor = refVoltage / ADC_FULL_SCALE;
}

uint8_t LTC2499::setVRef(float refVoltage, ADCChannel_t channel) {
  int32_t rawVRef = readRaw(channel);
  conversionFactor = refVoltage / rawVRef;
}

int32_t LTC2499::readRaw(ADCChannel_t channel) {
  char buf[4] = {channel, ADC_CONFIG_EXT_50_60_1x, 0, 0};
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
  uint32_t rawValue = (buf[3] << 18);
  rawValue |= (buf[2] << 10);
  rawValue |= (buf[1] << 2);
  rawValue |= (buf[0] >> 6);
  rawValue = rawValue & ADC_RAW_MASK;

  if (prefixBits == 0x03) {
    DEBUG("LTC2499", "Channel %d is overrange", channel);
    return ADC_OVERRANGE;
  }

  if (prefixBits == 0x00) {
    DEBUG("LTC2499", "Channel %d is underrange", channel);
    return ADC_UNDERRANGE;
  }

  return rawValue;
}