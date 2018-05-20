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
 * @date 6 Mar 2018
 * @brief Starts the IHU software
 *
 * Initializes IHU object and starts the eventQueue
 */

#include <mbed.h>
#include <rtos.h>
#include "tools/CISError.h"
#include "tools/CISConsole.h"
#include "PMICPins.h"
#include "drivers/LTC2499.h"

I2C i2cLocal(I2C_LOCAL_SDA, I2C_LOCAL_SCL);
LTC2499 adcB(i2cLocal, 0x2A, (1.024 / 2.0));

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  DEBUG("PMIC", "Initialization starting");
  DEBUG("PMIC", "Initialization complete");
  uint8_t result = 0;
  float temperature;
  result = adcB.readInternalTemperaure(&temperature);
  if (result != ERROR_SUCCESS) {
    DEBUG("ADC-B", "Failed to read internal temperature: 0x%02X", result);
  }
  DEBUG("ADC-B", "Internal temperature: %4.1fC", temperature);
  return ERROR_SUCCESS;
}
