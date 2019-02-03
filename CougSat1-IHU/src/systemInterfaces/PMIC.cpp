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
 * @author Bradley Davis, Kevin Evans
 * @date 3 Feb 2019
 * @brief Communicates betwen the IHU and the PMIC
 */

#include "PMIC.h"
#include "tools/CISError.h"
#include "tools/CISConsole.h"

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
 * @param target subsystem to toggle power for
 * @param on (or off)
 * @param result of the operation, passed from the PMIC
 * @return ERROR_SUCCESS on success,
 *         ERROR_INVALID_ARGS if the target is not supported for temperature
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestSetSubSystemPower(PMIC::TargetSubSystem_t target, bool on, uint8_t *result) {
  char buffer[2] = {
    on ? REQ_SUBSYSTEM_ON : REQ_SUBSYSTEM_OFF,
    target
  };

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 2) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error setting subsystem power for target %d (on: %d). No NACK after I2C write.", target, on);
    return ERROR_NACK;
  }

  if (this->i2c.read(I2C_ADDR_PMIC, (char *)result, 1) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error setting subsystem power for target %d (on: %d). No NACK after I2C read.", target, on);
    return ERROR_NACK;
  }

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC to retrieve voltage data for a target
 * @param target for pulling voltage data
 * @param output from the PMIC, a 2-byte array
 * @return ERROR_SUCCESS on success,
 *         ERROR_INVALID_ARGS if the target is not supported for temperature
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestGetVoltageData(PMIC::TargetReading_t target, uint8_t *output) {
  if (target > REGULATOR_B_3V3) {
    return ERROR_INVALID_ARGS;
  }

  char buffer[2] = {
    REQ_VOLTAGE_DATA,
    target
  };

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 2) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error getting voltage data for target %d. No NACK after I2C write.", target);
    return ERROR_NACK;
  }

  if (this->i2c.read(I2C_ADDR_PMIC, (char *)output, 2) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error getting voltage data for target %d. No NACK after I2C read.", target);
    return ERROR_NACK;
  }

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC to retrieve current data for a target
 * @param target for pulling current
 * @param output from the PMIC, a 2 byte array
 * @return ERROR_SUCCESS on success,
 *         ERROR_INVALID_ARGS if the target is not supported for temperature
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestGetCurrentData(PMIC::TargetReading_t target, uint8_t *output) {
  if (target > MPPT_7) {
    return ERROR_INVALID_ARGS;
  }

  char buffer[2] = {
    REQ_CURRENT_DATA,
    target
  };

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 2) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error getting current data for target %d. No NACK after I2C write.", target);
    return ERROR_NACK;
  }

  if (this->i2c.read(I2C_ADDR_PMIC, (char *)output, 2) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error getting voltage data for target %d. No NACK after I2C read.", target);
    return ERROR_NACK;
  }

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC to retrieve temperature data for a target
 * @param target for pulling temperature data
 * @param temperature of the target, in degrees celsius
 * @return ERROR_SUCCESS on success,
 *         ERROR_INVALID_ARGS if the target is not supported for temperature
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestGetTemperatureData(PMIC::TargetReading_t target, int8_t *temperature) {
  if (target < TARGET_PMIC && target > REGULATOR_B_3V3) {
    return ERROR_INVALID_ARGS;
  }

  char buffer[2] = {
    REQ_TEMPERATURE_DATA,
    target
  };

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 2) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error getting temperature data for target %d. No NACK after I2C write.", target);
    return ERROR_NACK;
  }

  if (this->i2c.read(I2C_ADDR_PMIC, (char *)temperature, 1) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error getting voltage data for target %d. No NACK after I2C read.", target);
    return ERROR_NACK;
  }

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC for reading power channel data.
 * @param status bit flags of the status of each power channel
 * @return ERROR_SUCCESS on success,
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestGetPowerChannelStatus(uint64_t *status) {
  char buffer[1] = {
    REQ_POWER_CHANNEL_STATUS
  };
  status = 0;

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 1) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error getting power channel status. No NACK after I2C write.");
    return ERROR_NACK;
  }

  if (this->i2c.read(I2C_ADDR_PMIC, (char *)status, 7) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error getting power channel status. No NACK after I2C read.");
    return ERROR_NACK;
  }

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC for reading solar panel channel data.
 * @param status bit flags of the status of each solar panel channel
 * @return ERROR_SUCCESS on success,
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestGetSolarPanelChannelStatus(uint16_t *status) {
  char buffer[1] = {
    REQ_SOLAR_PANEL_CHANNEL_STATUS
  };
  status = 0;

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 1) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error getting solar panel channel status. No NACK after I2C write.");
    return ERROR_NACK;
  }

  if (this->i2c.read(I2C_ADDR_PMIC, (char *)status, 2) != ERROR_SUCCESS) {
    DEBUG("IHU<->PMIC", "Error getting solar panel channel status. No NACK after I2C read.");
    return ERROR_NACK;
  }

  return ERROR_SUCCESS;
}
