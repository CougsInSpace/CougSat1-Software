/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file TCA9535.cpp
 * @author Bradley Davis
 * @date 3 Nov 2018
 * @brief Driver to talk to a TCA9535 ADC
 *
 */
#include "TCA9535.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

/**
 * @brief Construct a new TCA9535::TCA9535 object
 *
 * @param i2c connected to the GPIO
 * @param addr of the GPIO
 */
TCA9535::TCA9535(I2C & i2c, uint8_t addr) : i2c(i2c) {
  this->addr = addr;
}

/**
 * @brief Configures the direction, polarity, and value (if output)
 *
 * @param pin
 * @param input configures the pin to an input if true
 * @param polarityInversion inverts the pin if true
 * @param value is high if true
 * @return uint8_t error code
 */
uint8_t TCA9535::configurePin(
    GPIOExpanderPin_t pin, bool input, bool polarityInversion, bool value) {
  uint8_t mask     = 1 << (pin & 0x0F);
  uint8_t regValue = 0;
  uint8_t result   = write(pin, value);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Could not write pin to 0x%02X", addr);
    return result;
  }

  result = readRegister(POLARITY, pin, &regValue);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Could not read polarity register from 0x%02X", addr);
    return result;
  }
  if (polarityInversion) {
    regValue = regValue | mask;
  } else {
    regValue = regValue & ~(mask);
  }
  result = writeRegister(POLARITY, pin, regValue);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Could not write polarity register to 0x%02X", addr);
    return result;
  }

  result = readRegister(CONFIG, pin, &regValue);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Could not read config register from 0x%02X", addr);
    return result;
  }
  if (input) {
    regValue = regValue | mask;
  } else {
    regValue = regValue & ~(mask);
  }
  result = writeRegister(CONFIG, pin, regValue);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Could not write config register to 0x%02X", addr);
    return result;
  }

  return ERROR_SUCCESS;
}

/**
 * @brief Write to a pin, no effect if an input
 *
 * @param pin
 * @param value is high if true
 * @return uint8_t error code
 */
uint8_t TCA9535::write(GPIOExpanderPin_t pin, bool value) {
  uint8_t mask     = 1 << (pin & 0x0F);
  uint8_t regValue = 0;
  uint8_t result   = readRegister(OUTPUT, pin, &regValue);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Could not read output register from 0x%02X", addr);
    return result;
  }
  if (value) {
    regValue = regValue | mask;
  } else {
    regValue = regValue & ~(mask);
  }
  result = writeRegister(OUTPUT, pin, regValue);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Could not write output register to 0x%02X", addr);
    return result;
  }

  bool actualValue = false;

  result = read(pin, &actualValue);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Could not read 0x%02X from 0x%02X", pin, addr);
    return result;
  }
  if (value != actualValue) {
    DEBUG("TCA9535", "Output 0x%02X has not taken affect on 0x%02X", pin, addr);
    return ERROR_WRITE;
  }

  return ERROR_SUCCESS;
}

/**
 * @brief Reads from a pin
 * An output pin will return which state it was requested to be in
 *
 * @param pin
 * @param value is high if true
 * @return uint8_t error code
 */
uint8_t TCA9535::read(GPIOExpanderPin_t pin, bool * value) {
  uint8_t mask     = 1 << (pin & 0x0F);
  uint8_t regValue = 0;
  uint8_t result   = readRegister(INPUT, pin, &regValue);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Could not read input register from 0x%02X", addr);
    return result;
  }
  (*value) = (regValue & mask) != 0;

  return ERROR_SUCCESS;
}

/**
 * @brief Writes to a register
 *
 * @param reg to write to
 * @param value to write in
 * @return uint8_t error code
 */
uint8_t TCA9535::writeRegister(
    GPIOExpanderRegister_t reg, GPIOExpanderPin_t pin, uint8_t value) {
  char buf[2] = {(char)(reg | (pin >> 4)), (char)value};
  DEBUG("TCA9535", "Writing 0x%02X%02X to 0x%02X", buf[0], buf[1], addr);

  uint8_t result = i2c.write(addr, buf, 2);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Failed to write register 0x%02X to 0x%02X", buf[0], addr);
    return ERROR_NACK;
  }

  return ERROR_SUCCESS;
}

/**
 * @brief Reads from a register
 *
 * @param reg to read from
 * @param value to read into
 * @return uint8_t error code
 */
uint8_t TCA9535::readRegister(
    GPIOExpanderRegister_t reg, GPIOExpanderPin_t pin, uint8_t * value) {
  char buf[2] = {(char)(reg | (pin >> 4))};
  DEBUG("TCA9535", "Writing 0x%02X to 0x%02X", buf[0], addr, true);

  uint8_t result = i2c.write(addr, buf, 1);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Failed to command register 0x%02X from 0x%02X", buf[0], addr);
    i2c.stop();
    return ERROR_NACK;
  }

  result = i2c.read(addr, buf, 1);
  if (result != ERROR_SUCCESS) {
    DEBUG("TCA9535", "Failed to read register 0x%02X from 0x%02X", buf[0], addr);
    return ERROR_NACK;
  }
  (*value) = buf[0];

  return ERROR_SUCCESS;
}