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
I2C i2cBus(I2C_BUS_SDA, I2C_BUS_SCL);
I2C i2cPMIC(I2C_PMIC_SDA, I2C_PMIC_SCL);
// LTC2499 adcB(i2cLocal, 0x6A, (0.2094 / 2.0));

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  DEBUG("PMIC", "Initialization starting");
  DEBUG("PMIC", "Initialization complete");
  // float temperature;
  wait(1.0);
  // uint8_t result = adcB.readInternalTemperaure(&temperature);
  char buf[4] = {0, 0, 0, 0};
  uint8_t result = i2cLocal.write(0x6A, buf, 1);
  if(result != ERROR_SUCCESS){
    DEBUG("ADC-B", "Reading internal temp failed: 0x%02x", result);
    return result;
  }
  // DEBUG("ADC-B", "Internal temperature: %3.0fºC", temperature);
  DEBUG("LTC2499", "read 0x%02x 0x%02x 0x%02x 0x%02x", buf[0], buf[1], buf[2], buf[3]);
  return ERROR_SUCCESS;
}
