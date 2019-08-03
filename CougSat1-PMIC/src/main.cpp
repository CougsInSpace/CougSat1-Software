#include <CISConsole.h>
#include <mbed.h>

#include "Configuration.h"
#include "PMICObjects.h"
#include "events/Events.h"
#include "interfaces/CDH.h"

/**
 * @brief Initializes the all of the subclasses of the PMIC
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t initialize() {
  LOG("Init", "Initialization starting");
  mbed_error_status_t result = MBED_SUCCESS;

  // statusLED = 1;

  // result = gpioEPS0.configureAll(false, false, true);
  // if (result != ERROR_SUCCESS) {
  //   ERROR("Init", "Failed to configure gpioEPS0: 0x%02X", result);
  //   return result;
  // }

  // result = gpioEPS1.configureAll(false, false, true);
  // if (result != ERROR_SUCCESS) {
  //   ERROR("Init", "Failed to configure gpioEPS1: 0x%02X", result);
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
  //   DEBUG("Init", "Set switch nodesPVOut[%d]: 0x%02X", i, result);
  // }
  // LOG("Init", "nodesPVOut were set succesfully");
  // for (int i = 0; i < COUNT_PR_3V3; i++) {
  //   result = nodesPR3V3[i]->setSwitch(pathA, pathB);
  //   if (result != ERROR_SUCCESS) {
  //     ERROR("Init", "Failed to switch nodesPR3V3[%d]: 0x%02X", i, result);
  //     return result;
  //   }
  //   DEBUG("Init", "Set switch nodesPR3V3[%d]: 0x%02X", i, result);
  // }
  // LOG("Init", "nodesPR3V3 were set succesfully");
  // for (int i = 0; i < COUNT_PR_BATT; i++) {
  //   result = nodesPRBatt[i]->setSwitch(pathA, pathB);
  //   if (result != ERROR_SUCCESS) {
  //     ERROR("Init", "Failed to switch nodesPRBatt[%d]: 0x%02X", i, result);
  //     return result;
  //   }
  //   DEBUG("Init", "Set switch nodesPRBatt[%d]: 0x%02X", i, result);
  // }
  // LOG("Init", "nodesPRBatt were set succesfully");
  // for (int i = 0; i < COUNT_PV_3V3; i++) {
  //   result = nodesPV3V3[i]->setSwitch(pathA, pathB);
  //   if (result != ERROR_SUCCESS) {
  //     ERROR("Init", "Failed to switch nodesPV3V3[%d]: 0x%02X", i, result);
  //     return result;
  //   }
  // }
  // LOG("Init", "nodesPV3V3 were set succesfully");
  // for (int i = 0; i < COUNT_BH; i++) {
  //   result = nodesBatteryHeaters[i]->setSwitch(pathA, pathB);
  //   if (result != ERROR_SUCCESS) {
  //     ERROR("Init", "Failed to switch nodesBH[%d]: 0x%02X ", i, result);
  //     return result;
  //   }
  // }
  // LOG("Init", "nodesBatteryHeaters were set succesfully");
  // result = nodeDeployables.setSwitch(pathA, pathB);
  // if (result != ERROR_SUCCESS) {
  //   ERROR("Init", "Failed to switch nodeDeployable: 0x%02X", result);
  //   return result;
  // }
  // LOG("Init", "nodeDeployable was set succesfully");

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
  return result;
}

/**
 * @brief Exectutes the main loop of the PMIC
 *
 * @return mbed_error_status_t error code
 */
mbed_error_status_t run() {
  uint32_t now               = HAL_GetTick();
  uint32_t nextPeriodicEvent = now + PERIOD_MS_PERIODIC;

  mbed_error_status_t result = MBED_SUCCESS;
  while (true) {
    now = HAL_GetTick();
    if (now >= nextPeriodicEvent && (nextPeriodicEvent >= PERIOD_MS_PERIODIC ||
                                        now <= PERIOD_MS_PERIODIC)) {
      result = eventPeriodic();
      if (result) {
        ERROR("Run", "Failed to perform periodic event: 0x%02X", result);
        return result;
      }
      nextPeriodicEvent = now + PERIOD_MS_PERIODIC;
    } else if (cdh.hasMessage()) {
      result = cdh.processMessage();
      if (result) {
        ERROR("Run", "Failed to process message from the bus: 0x%02X", result);
        return result;
      }
    } else {
      wait_ms(PERIOD_MS_IDLE_SLEEP);
    }
  }
}

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  mbed_error_status_t result = initialize();
  if (result) {
    ERROR("PMIC", "Failed to initialize: 0x%02X", result);
  }
  return MBED_SUCCESS;
}
