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
 * @author Kevin Evans
 * @date 25 Jan 2019
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
 * @param target subsystem to toggle power for
 * @param on (or off)
 * @param result of the operation, passed from the PMIC
 * @return ERROR_SUCCESS on success,
 *         ERROR_INVALID_ARGS if the target is not supported for temperature
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestSetSubSystemPower(PMIC::TargetSubSystem_t target, bool on, uint8_t &result) {
  char buffer[2] = {
    on ? PMIC_COMMAND_REQ_SUBSYSTEM_ON : PMIC_COMMAND_REQ_SUBSYSTEM_OFF,
    target
  };

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 2) != 0) {
    return ERROR_WRITE;
  }

  if (this->i2c.read(I2C_ADDR_PMIC, (char *)&result, 1) != 0) {
    return ERROR_READ;
  }

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC to retrieve voltage data for a target
 * @param target for pulling voltage data
 * @param voltage of the target, in volts
 * @return ERROR_SUCCESS on success,
 *         ERROR_INVALID_ARGS if the target is not supported for temperature
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestGetVoltageData(PMIC::TargetReading_t target, float &voltage) {
  if (target > REGULATOR_B_3V3) {
    return ERROR_INVALID_ARGS;
  }

  char buffer[2] = {
    PMIC_COMMAND_REQ_VOLTAGE_DATA,
    target
  };
  unsigned short response;

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 2) != 0) {
    return ERROR_WRITE;
  }

  if (this->i2c.read(I2C_ADDR_PMIC, (char *)&response, 2) != 0) {
    return ERROR_READ;
  }

  voltage = (float)response * PMIC_VOLTAGE_RESOLUTION;

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC to retrieve current data for a target
 * @param target for pulling current
 * @param current of target, in amps
 * @return ERROR_SUCCESS on success,
 *         ERROR_INVALID_ARGS if the target is not supported for temperature
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestGetCurrentData(PMIC::TargetReading_t target, float &current) {
  if (target > MPPT_7) {
    return ERROR_INVALID_ARGS;
  }

  char buffer[2] = {
    PMIC_COMMAND_REQ_CURRENT_DATA,
    target
  };
  unsigned short response;

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 2) != 0) {
    return ERROR_WRITE;
  }

  if (this->i2c.read(I2C_ADDR_PMIC, (char *)&response, 2) != 0) {
    return ERROR_READ;
  }

  current = (float)response * PMIC_CURRENT_RESOLUTION;

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
uint8_t PMIC::requestGetTemperatureData(PMIC::TargetReading_t target, int8_t &temperature) {
  if (target < TARGET_PMIC && target > REGULATOR_B_3V3) {
    return ERROR_INVALID_ARGS;
  }

  char buffer[2] = {
    PMIC_COMMAND_REQ_TEMPERATURE_DATA,
    target
  };

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 2) != 0) {
    return ERROR_WRITE;
  }

  if (this->i2c.read(I2C_ADDR_PMIC, (char *)&temperature, 1) != 0) {
    return ERROR_READ;
  }

  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC for reading power channel data.
 * @param status bit flags of the status of each power channel
 * @return ERROR_SUCCESS on success,
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestGetPowerChannelStatus(uint64_t &status) {
  char buffer[1] = {
    PMIC_COMMAND_REQ_POWER_CHANNEL_STATUS
  };
  status = 0;

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 1) != 0) {
    return ERROR_WRITE;
  }
  if (this->i2c.read(I2C_ADDR_PMIC, (char *)&status, 7) != 0) {
    return ERROR_READ;
  }
  return ERROR_SUCCESS;
}

/**
 * Sends a request to the PMIC for reading solar panel channel data.
 * @param status bit flags of the status of each solar panel channel
 * @return ERROR_SUCCESS on success,
 *         ERROR_WRITE on I2C write failure
 */
uint8_t PMIC::requestGetSolarPanelChannelStatus(uint16_t &status) {
  char buffer[1] = {
    PMIC_COMMAND_REQ_SOLAR_PANEL_CHANNEL_STATUS
  };
  status = 0;

  if (this->i2c.write(I2C_ADDR_PMIC, buffer, 1) != 0) {
    return ERROR_WRITE;
  }
  if (this->i2c.read(I2C_ADDR_PMIC, (char *)&status, 2) != 0) {
    return ERROR_READ;
  }
  return ERROR_SUCCESS;
}
