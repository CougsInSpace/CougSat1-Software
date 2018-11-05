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
TCA9535::TCA9535(I2C &i2c, uint8_t addr) : i2c(i2c) { this->addr = addr; }

/**
 * @brief Configures the direction, polarity, and value (if output)
 *
 * @param pin
 * @param input configures the pin to an input if true
 * @param polarityInversion inverts the pin if true
 * @param value is high if true
 * @return uint8_t error code
 */
uint8_t TCA9535::configurePin(GPIOExpanderPin_t pin, bool input,
                              bool polarityInversion, bool value) {
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Write to a pin, no effect if an input
 *
 * @param pin
 * @param value is high if true
 * @return uint8_t error code
 */
uint8_t TCA9535::write(GPIOExpanderPin_t pin, bool value) {
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Reads from a pin
 * An output pin will return which state it was requested to be in
 *
 * @param pin
 * @param value is high if true
 * @return uint8_t error code
 */
uint8_t TCA9535::read(GPIOExpanderPin_t pin, bool *value) {
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Writes to a register
 *
 * @param reg to write to
 * @param value to write in
 * @return uint8_t error code
 */
uint8_t TCA9535::writeRegister(GPIOExpanderRegister_t reg, uint8_t value) {
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Reads from a register
 *
 * @param reg to read from
 * @param value to read into
 * @return uint8_t error code
 */
uint8_t TCA9535::readRegister(GPIOExpanderRegister_t reg, uint8_t *value) {
  return ERROR_NOT_SUPPORTED;
}