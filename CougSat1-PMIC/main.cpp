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
#include "mbed.h"
#include "PMICObjects.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

/**
 * @brief Initializes the all of the subclasses of the PMIC
 *
 * @return uint8_t error code
 */
uint8_t initialize()
{
  DEBUG("PMIC", "Initialization starting");
  DEBUG("PMIC", "Initialization complete");
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Exectutes the main loop of the PMIC
 *
 * @return uint8_t error code
 */
uint8_t run() { return ERROR_NOT_SUPPORTED; }

/**
 * Program start routine
 * @return error code
 */
int main(void)
{
  uint8_t result = initialize();
  if (result != ERROR_SUCCESS)
  {
    DEBUG("PMIC", "Failed to initialize PMIC: %02X", result);
  }

  result = run();
  if (result != ERROR_SUCCESS)
  {
    DEBUG("PMIC", "Failed to run main loop: %02X", result);
  }
  return result;
}
