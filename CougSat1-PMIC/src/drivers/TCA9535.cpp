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
 * @return error code
 */
uint8_t TCA9535::configurePin(
    TCA9535::Pin_t pin, bool input, bool polarityInversion, bool value) {
  uint8_t mask     = 1 << (pin & 0x0F);
  uint8_t regValue = 0;
  uint8_t result   = write(pin, value);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write pin to 0x%02X", addr);
    return result;
  }

  result = readRegister(TCA9535::POLARITY, pin, &regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not read polarity register from 0x%02X", addr);
    return result;
  }
  if (polarityInversion) {
    regValue = regValue | mask;
  } else {
    regValue = regValue & ~(mask);
  }
  result = writeRegister(TCA9535::POLARITY, pin, regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write polarity register to 0x%02X", addr);
    return result;
  }

  result = readRegister(TCA9535::CONFIG, pin, &regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not read config register from 0x%02X", addr);
    return result;
  }
  if (input) {
    regValue = regValue | mask;
  } else {
    regValue = regValue & ~(mask);
  }
  result = writeRegister(TCA9535::CONFIG, pin, regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write config register to 0x%02X", addr);
    return result;
  }

  return ERROR_SUCCESS;
}

uint8_t TCA9535::configureAll(bool input, bool polarityInversion, bool value) {
  uint8_t regValue = 0;
  uint8_t result   = 0;
  if (value) {
    regValue = 0xFF;
  } else {
    regValue = 0x00;
  }
  result = writeRegister(TCA9535::OUTPUT, TCA9535::P00, regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write output register to 0x%02X", addr);
    return result;
  }
  result = writeRegister(TCA9535::OUTPUT, TCA9535::P10, regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write output register to 0x%02X", addr);
    return result;
  }

  if (polarityInversion) {
    regValue = 0xFF;
  } else {
    regValue = 0x00;
  }
  result = writeRegister(TCA9535::POLARITY, TCA9535::P00, regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write polarity register to 0x%02X", addr);
    return result;
  }
  result = writeRegister(TCA9535::POLARITY, TCA9535::P10, regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write polarity register to 0x%02X", addr);
    return result;
  }

  if (input) {
    regValue = 0xFF;
  } else {
    regValue = 0x00;
  }
  result = writeRegister(TCA9535::CONFIG, TCA9535::P00, regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write config register to 0x%02X", addr);
    return result;
  }
  result = writeRegister(TCA9535::CONFIG, TCA9535::P10, regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write config register to 0x%02X", addr);
    return result;
  }

  return ERROR_SUCCESS;
}

uint8_t TCA9535::dumpRegisters() {
  uint8_t regValue = 0;
  uint8_t result   = 0;
  result = readRegister(TCA9535::OUTPUT, TCA9535::P00, &regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write output register to 0x%02X", addr);
    return result;
  }
  LOG("TCA9535", "Port 0 of 0x%02X's output is 0x%02X", addr, regValue);
  result = readRegister(TCA9535::OUTPUT, TCA9535::P10, &regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write output register to 0x%02X", addr);
    return result;
  }
  LOG("TCA9535", "Port 1 of 0x%02X's output is 0x%02X", addr, regValue);

  result = readRegister(TCA9535::POLARITY, TCA9535::P00, &regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write polarity register to 0x%02X", addr);
    return result;
  }
  LOG("TCA9535", "Port 0 of 0x%02X's polarity is 0x%02X", addr, regValue);
  result = readRegister(TCA9535::POLARITY, TCA9535::P10, &regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write polarity register to 0x%02X", addr);
    return result;
  }
  LOG("TCA9535", "Port 1 of 0x%02X's polarity is 0x%02X", addr, regValue);

  result = readRegister(TCA9535::CONFIG, TCA9535::P00, &regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write config register to 0x%02X", addr);
    return result;
  }
  LOG("TCA9535", "Port 0 of 0x%02X's config is 0x%02X", addr, regValue);
  result = readRegister(TCA9535::CONFIG, TCA9535::P10, &regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write config register to 0x%02X", addr);
    return result;
  }
  LOG("TCA9535", "Port 1 of 0x%02X's config is 0x%02X", addr, regValue);

  return ERROR_SUCCESS;
}

/**
 * @brief Write to a pin, no effect if an input
 *
 * @param pin
 * @param value is high if true
 * @return uint8_t error code
 */
uint8_t TCA9535::write(TCA9535::Pin_t pin, bool value) {
  uint8_t mask     = 1 << (pin & 0x0F);
  uint8_t regValue = 0;
  uint8_t result   = readRegister(TCA9535::OUTPUT, pin, &regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not read output register from 0x%02X", addr);
    return result;
  }
  if (value) {
    regValue = regValue | mask;
  } else {
    regValue = regValue & ~(mask);
  }
  result = writeRegister(TCA9535::OUTPUT, pin, regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not write output register to 0x%02X", addr);
    return result;
  }

  bool actualValue = false;

  result = read(pin, &actualValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not read 0x%02X from 0x%02X", pin, addr);
    return result;
  }
  if (value != actualValue) {
    ERROR("TCA9535", "Output 0x%02X has not taken affect on 0x%02X", pin, addr);
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
uint8_t TCA9535::read(TCA9535::Pin_t pin, bool * value) {
  uint8_t mask     = 1 << (pin & 0x0F);
  uint8_t regValue = 0;
  uint8_t result   = readRegister(TCA9535::INPUT, pin, &regValue);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Could not read input register from 0x%02X", addr);
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
    TCA9535::Register_t reg, TCA9535::Pin_t pin, uint8_t value) {
  char buf[2] = {(char)(reg | (pin >> 4)), (char)value};
  DEBUG("TCA9535", "Writing 0x%02X%02X to 0x%02X", buf[0], buf[1], addr);

  uint8_t result = i2c.write(addr, buf, 2);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Failed to write register 0x%02X to 0x%02X", buf[0], addr);
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
    TCA9535::Register_t reg, TCA9535::Pin_t pin, uint8_t * value) {
  char buf[1] = {(char)(reg | (pin >> 4))};
  DEBUG("TCA9535", "Writing 0x%02X to 0x%02X", buf[0], addr);

  uint8_t result = i2c.write(addr, buf, 1, true);
  if (result != ERROR_SUCCESS) {
    ERROR("TCA9535", "Failed to command register 0x%02X from 0x%02X", buf[0],
        addr);
    i2c.stop();
    return ERROR_NACK;
  }

  result = i2c.read(addr, buf, 1);
  if (result != ERROR_SUCCESS) {
    ERROR(
        "TCA9535", "Failed to read register 0x%02X from 0x%02X", buf[0], addr);
    return ERROR_NACK;
  }
  DEBUG("TCA9535", "Read 0x%02X from 0x%02X", buf[0], addr);
  (*value) = buf[0];

  return ERROR_SUCCESS;
}