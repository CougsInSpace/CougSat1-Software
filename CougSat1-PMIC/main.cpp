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
#include "PMICConfiguration.h"
#include "PMICObjects.h"
#include "PMICPins.h"
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
  LOG("Init", "Initialization starting");
  uint8_t result = 0;

  led = 1;

  // result = gpioEPS0.configureAll(false, true, false);
  // if (result != ERROR_SUCCESS) {
  //   ERROR("Init", "Failed to configure gpioEPS0: 0x%02X", result);
  //   return result;
  // }
  // nodesPRBatt[3]->setSwitch(true, true);
  // nodesPR3V3[7]->setSwitch(true, true);
  // nodesPR3V3[11]->setSwitch(true, true);

  result = gpioEPS0.configureAll(false, true, false);
  if (result != ERROR_SUCCESS) {
    ERROR("Init", "Failed to configure gpioEPS1: 0x%02X", result);
    return result;
  }

  result = gpioEPS0.dumpRegisters();
  if (result != ERROR_SUCCESS) {
    ERROR("Init", "Failed to configure gpioEPS1: 0x%02X", result);
    return result;
  }


  // result = gpioPV0.configureAll(false, false, true);
  // if (result != ERROR_SUCCESS) {
  //   ERROR("Init", "Failed to configure gpioPV0: 0x%02X", result);
  //   return result;
  // }

  // result = gpioPV1.configureAll(false, false, true);
  // if (result != ERROR_SUCCESS) {
  //   ERROR("Init", "Failed to configure gpioPV1: 0x%02X", result);
  //   return result;
  // }

  // result = gpioPV2.configureAll(false, false, true);
  // if (result != ERROR_SUCCESS) {
  //   ERROR("Init", "Failed to configure gpioPV2: 0x%02X", result);
  //   return result;
  // }

  // result = gpioPV3.configureAll(false, false, true);
  // if (result != ERROR_SUCCESS) {
  //   ERROR("Init", "Failed to configure gpioPV3: 0x%02X", result);
  //   return result;
  // }

  // bool pathA = false;
  // bool pathB = false;
  // LOG("Init", "All power nodes are A: %s, B %s", pathA ? "on" : "off",
  //     pathB ? "on" : "off");
  // for (int i = 0; i < COUNT_PV; i++) {
  //   result = nodesPVOut[i]->setSwitch(pathA, pathB);
  //   if (result != ERROR_SUCCESS) {
  //     ERROR("Init", "Failed to switch nodesPVOut[%d]: 0x%02X", i, result);
  //     return result;
  //   }
  // }
  // for (int i = 0; i < COUNT_PR_3V3; i++) {
  //   result = nodesPR3V3[i]->setSwitch(pathA, pathB);
  //   if (result != ERROR_SUCCESS) {
  //     ERROR("Init", "Failed to switch nodesPR3V3[%d]: 0x%02X", i, result);
  //     return result;
  //   }
  // }
  // for (int i = 0; i < COUNT_PR_BATT; i++) {
  //   result = nodesPRBatt[i]->setSwitch(pathA, pathB);
  //   if (result != ERROR_SUCCESS) {
  //     ERROR("Init", "Failed to switch nodesPRBatt[%d]: 0x%02X", i, result);
  //     return result;
  //   }
  // }
  // for (int i = 0; i < COUNT_PV_3V3; i++) {
  //   result = nodesPV3V3[i]->setSwitch(pathA, pathB);
  //   if (result != ERROR_SUCCESS) {
  //     ERROR("Init", "Failed to switch nodesPV3V3[%d]: 0x%02X", i, result);
  //     return result;
  //   }
  // }
  // for (int i = 0; i < COUNT_BH; i++) {
  //   result = nodesBatteryHeaters[i]->setSwitch(pathA, pathB);
  //   if (result != ERROR_SUCCESS) {
  //     ERROR("Init", "Failed to switch nodesBH[%d]: 0x%02X ", i, result);
  //     return result;
  //   }
  // }
  // result = nodeDeployables.setSwitch(pathA, pathB);
  // if (result != ERROR_SUCCESS) {
  //   ERROR("Init", "Failed to switch nodeDeployable: 0x%02X", result);
  //   return result;
  // }

  // double value = 0.0;
  // result       = adcEPS5.readVoltage(PIN_ADC_EJECT_TIMER, &value);
  // if (result != ERROR_SUCCESS) {
  //   ERROR("Init", "Failed to read eject timer voltage: 0x%02X", result);
  //   return result;
  // }
  // if (value < THRES_EJECT_TIMER) {
  //   LOG("Init", "First boot detected: %5.3fV", value);
  //   result = eventFirstBoot();
  //   if (result != ERROR_SUCCESS) {
  //     ERROR("Init", "Failed to perform first boot: 0x%02X", result);
  //     return result;
  //   }
  // } else {
  //   LOG("Init", "First boot not detected: %5.3fV", value);
  // }

  LOG("Init", "Initialization complete");
  return ERROR_INVALID_DATA;
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
        ERROR("Run", "Failed to process message from the bus: 0x%02X", result);
      }
    }
    now = HAL_GetTick();
    if (now >= nextADCEvent &&
        (nextADCEvent >= PERIOD_MS_ADC_UPDATE || now <= PERIOD_MS_ADC_UPDATE)) {
      led    = !led;
      result = eventADC();
      if (result != ERROR_SUCCESS) {
        ERROR("Run", "Failed to perform ADC event: 0x%02X", result);
      }
      nextADCEvent = HAL_GetTick() + PERIOD_MS_ADC_UPDATE;
    }
    now = HAL_GetTick();
    if (now >= nextPeriodicEvent && (nextPeriodicEvent >= PERIOD_MS_PERIODIC ||
                                        now <= PERIOD_MS_PERIODIC)) {
      result = eventPeriodic();
      if (result != ERROR_SUCCESS) {
        ERROR("Run", "Failed to perform periodic event: 0x%02X", result);
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
    ERROR("PMIC", "Failed to initialize: 0x%02X", result);
    return result;
  }

  result = run();
  if (result != ERROR_SUCCESS) {
    ERROR("PMIC", "Failed to run main loop: 0x%02X", result);
    return result;
  }
  return ERROR_SUCCESS;
}
