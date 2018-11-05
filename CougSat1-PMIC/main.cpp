/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file main.cpp
 * @author Bradley Davis
 * @date 4 Nov 2018
 * @brief Starts the PMIC software
 *
 * Initializes PMIC object and starts the execution
 */

#include "PMIC.h"
#include "mbed.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"
#include "drivers/LTC2499.h"
#include "PMICPins.h"

PMIC pmic;

I2C i2cLocal(PIN_I2C_BUS_SDA, PIN_I2C_BUS_SCL);
I2C i2cBus(PIN_I2C_LOCAL_SDA, PIN_I2C_LOCAL_SCL);

LTC2499 adcEPS0(i2cBus, I2C_ADDR_ADC_EPS0);
/**
 * Program start routine
 * @return error code
 */
int main(void) {
  uint8_t result = pmic.initialize();
  if (result != ERROR_SUCCESS) {
    DEBUG("PMIC", "Failed to initialize PMIC: %02X", result);
  }

  result = pmic.run();
  if (result != ERROR_SUCCESS) {
    DEBUG("PMIC", "Failed to run main loop: %02X", result);
  }

  result = pmic.stop();
  if (result != ERROR_SUCCESS) {
    DEBUG("PMIC", "Failed to stop PMIC: %02X", result);
  }

  return result;
}
