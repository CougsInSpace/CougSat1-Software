/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PMIC.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates betwen the IHU and the PMIC
 */

#include "PMIC.h"
#include "tools/CISError.h"

/**
 * Constructs PMIC object
 * Saves a reference to the i2c
 * @param i2c connected to PMIC
 */
PMIC::PMIC(I2C &i2c) :
    i2c(i2c) {

}

/**
 * Destructor is currently empty
 */
PMIC::~PMIC() {

}

/**
 * Initializes interface to PMIC
 * @return error code
 */
uint8_t PMIC::initialize() {
  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC to toggle power to a specific subsystem.
 * @return error code
 */
uint8_t PMIC::requestSetSubSystemPower(PMIC::TargetSubsystem target, bool on) {
  char buffer[2] = {
    on,
    target
  };

  this->i2c.write(I2C_ADDR_PMIC, buffer, 2);

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC to retrieve voltage data for a target
 * @return error code
 */
uint8_t PMIC::requestGetVoltageData(PMIC::TargetReading target) {
  if (target >= PR_3V3_0) {
    return ERROR_INVALID_ARGS;
  }

  char buffer[2] = {
    0x02,
    target
  };

  this->i2c.write(I2C_ADDR_PMIC, buffer, 2);

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC to retrieve current data for a target
 * @return error code
 */
uint8_t PMIC::requestGetCurrentData(PMIC::TargetReading target) {
  if (target >= PR_DEPLOY) {
    return ERROR_INVALID_ARGS;
  }

  char buffer[2] = {
    0x03,
    target
  };

  this->i2c.write(I2C_ADDR_PMIC, buffer, 2);

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC to retrieve temperature data for a target
 * @return error code
 */
uint8_t PMIC::requestGetTempData(PMIC::TargetReading target) {
  if (target < TargetPMIC && target > RegulatorB3V3) {
    return ERROR_INVALID_ARGS;
  }

  char buffer[2] = {
    0x04,
    target
  };

  this->i2c.write(I2C_ADDR_PMIC, buffer, 2);

  return ERROR_SUCCESS;
}

