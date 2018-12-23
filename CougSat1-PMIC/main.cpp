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
#include "PMICObjects.h"
#include "events/Events.h"
#include "mbed.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

volatile bool busMessage = false;

/**
 * @brief Initializes the all of the subclasses of the PMIC
 *
 * @return uint8_t error code
 */
uint8_t initialize() {
  DEBUG("Init", "Initialization starting");
  uint8_t result = 0;

  bool pathA = true;
  bool pathB = false;
  DEBUG("Init", "All power nodes are A: %s, B %s", pathA ? "on" : "off",
      pathB ? "on" : "off");
  for (int i = 0; i < COUNT_PV; i++) {
    result = nodesPVOut[i]->setSwitch(pathA, pathB);
    if (result != ERROR_SUCCESS) {
      DEBUG("Init", "Failed to switch nodesPVOut[%d]: 0x%02X", i, result);
      return result;
    }
  }
  for (int i = 0; i < COUNT_PR_3V3; i++) {
    result = nodesPR3V3[i]->setSwitch(pathA, pathB);
    if (result != ERROR_SUCCESS) {
      DEBUG("Init", "Failed to switch nodesPR3V3[%d]: 0x%02X", i, result);
      return result;
    }
  }
  for (int i = 0; i < COUNT_PR_BATT; i++) {
    result = nodesPRBatt[i]->setSwitch(pathA, pathB);
    if (result != ERROR_SUCCESS) {
      DEBUG("Init", "Failed to switch nodesPRBatt[%d]: 0x%02X", i, result);
      return result;
    }
  }
  for (int i = 0; i < COUNT_PV_3V3; i++) {
    result = nodesPV3V3[i]->setSwitch(pathA, pathB);
    if (result != ERROR_SUCCESS) {
      DEBUG("Init", "Failed to switch nodesPV3V3[%d]: 0x%02X", i, result);
      return result;
    }
  }
  for (int i = 0; i < COUNT_BH; i++) {
    result = nodesBH[i]->setSwitch(pathA, pathB);
    if (result != ERROR_SUCCESS) {
      DEBUG("Init", "Failed to switch nodesBH[%d]: 0x%02X ", i, result);
      return result;
    }
  }
  result = nodeDeployables.setSwitch(pathA, pathB);
  if (result != ERROR_SUCCESS) {
    DEBUG("Init", "Failed to switch nodeDeployable: 0x%02X", result);
    return result;
  }

  DEBUG("Init", "Initialization complete");
  return ERROR_SUCCESS;
}

/**
 * @brief Exectutes the main loop of the PMIC
 *
 * @return uint8_t error code
 */
uint8_t run() {
  uint32_t nextADCEvent      = HAL_GetTick() + PERIOD_MS_ADC_UPDATE;
  uint32_t nextPeriodicEvent = HAL_GetTick() + PERIOD_MS_PERIODIC;
  uint32_t now               = HAL_GetTick();

  uint8_t result = 0;
  while (true) {
    if (busMessage) {
      result = cdh.processMessage();
      if (result != ERROR_SUCCESS) {
        DEBUG("Run", "Failed to process message from the bus: 0x%02X", result);
      }
    }
    now = HAL_GetTick();
    if (now >= nextADCEvent &&
        (nextADCEvent >= PERIOD_MS_ADC_UPDATE || now <= PERIOD_MS_ADC_UPDATE)) {
      result = eventADC();
      if (result != ERROR_SUCCESS) {
        DEBUG("Run", "Failed to perform ADC event: 0x%02X", result);
      }
      nextADCEvent = HAL_GetTick() + PERIOD_MS_ADC_UPDATE;
    }
    now = HAL_GetTick();
    if (now >= nextPeriodicEvent && (nextPeriodicEvent >= PERIOD_MS_PERIODIC ||
                                        now <= PERIOD_MS_PERIODIC)) {
      result = eventPeriodic();
      if (result != ERROR_SUCCESS) {
        DEBUG("Run", "Failed to perform periodic event: 0x%02X", result);
      }
      nextPeriodicEvent = HAL_GetTick() + PERIOD_MS_PERIODIC;
    }
    wait_ms(PERIOD_MS_IDLE_SLEEP);
  }
  return result;
}

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  uint8_t result = initialize();
  if (result != ERROR_SUCCESS) {
    DEBUG("PMIC", "Failed to initialize PMIC: 0x%02X", result);
    return result;
  }

  result = run();
  if (result != ERROR_SUCCESS) {
    DEBUG("PMIC", "Failed to run main loop: 0x%02X", result);
    return result;
  }
  return result;
}
