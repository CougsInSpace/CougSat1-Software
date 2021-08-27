#include <mbed.h>

#include <ADC/AD7291.h>
#include <ADC/AD7689.h>
#include <CISConsole.h>

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
  mbed_error_status_t error = MBED_SUCCESS;

  double value = 0.0;
  error        = adcEPSs[3]->readTemp(value);
  if (error) {
    ERROR("Init", "Failed to read temp ADC 3, 0x%08X", error);
    return error;
  }
  LOG("Init", "ADC 3 temp: %5.2f", value);

  error = ((AD7689 *)adcEPSs[0])->selfTest();
  if (error) {
    ERROR("Init", "Failed to self test ADC 0, 0x%08X", error);
    return error;
  }

  error = ((AD7689 *)adcEPSs[1])->selfTest();
  if (error) {
    ERROR("Init", "Failed to self test ADC 1, 0x%08X", error);
    return error;
  }

  error = ((AD7689 *)adcEPSs[2])->selfTest();
  if (error) {
    ERROR("Init", "Failed to self test ADC 2, 0x%08X", error);
    return error;
  }

  bool inputAOn = true;
  bool inputBOn = true;
  inputSwitching[0]->write(inputAOn);
  inputSwitching[1]->write(inputBOn);
  inputSwitching[2]->write(inputAOn);
  inputSwitching[3]->write(inputBOn);

  bool outputsOn = false;
  bool heatersOn = false;
  for (uint8_t i = 0; i < COUNT_PR_3V3; i++)
    nodesPR3V3[i]->setSwitch(outputsOn);
  for (uint8_t i = 0; i < COUNT_PR_BATT; i++)
    nodesPRBatt[i]->setSwitch(outputsOn);
  for (uint8_t i = 0; i < COUNT_DEPLOY; i++)
    nodesDeployables[i]->setSwitch(outputsOn);
  for (uint8_t i = 0; i < COUNT_BH; i++)
    nodesBatteryHeaters[i]->setSwitch(heatersOn);

  double ejectTimer = 0.0;
  // TODO Make VoltageNode and PowerNode classes
  error = adcEPSs[2]->readVoltage(ADCChannel_t::CM_02, ejectTimer);
  if (error) {
    ERROR("Init", "Failed to read eject timer, 0x%08X", error);
    return error;
  }
  bool ejected = ejectTimer < THRES_EJECT_TIMER;
  LOG("Init", "Eject timer: %9.5fV\tEjected: %s", ejectTimer,
      ejectTimer ? "true" : "false");

  if (ejected) {
    error = eventFirstBoot();
    if (error) {
      ERROR("Init", "Failed to perform first bool event: 0x%02X", error);
      return error;
    }
  }

  LOG("Init", "Initialization complete");
  return error;
}

/**
 * @brief Executes the main loop of the PMIC
 *
 * @return mbed_error_status_t error code
 */
mbed_error_status_t run() {
  uint32_t now               = HAL_GetTick();
  uint32_t nextPeriodicEvent = now + PERIOD_MS_PERIODIC;

  // mbed_error_status_t error = MBED_SUCCESS;
  while (true) {
    now = HAL_GetTick();
    if (now >= nextPeriodicEvent && (nextPeriodicEvent >= PERIOD_MS_PERIODIC ||
                                        now <= PERIOD_MS_PERIODIC)) {
      statusLED = !statusLED;
      // error     = eventPeriodic();
      // if (error) {
      //   ERROR("Run", "Failed to perform periodic event: 0x%02X", error);
      //   return error;
      // }
      nextPeriodicEvent = now + PERIOD_MS_PERIODIC;
      // } else if (cdh.hasMessage()) {
      //   error = cdh.processMessage();
      //   if (error) {
      //     ERROR("Run", "Failed to process message from the bus: 0x%02X",
      //     error); return error;
      //   }
    } else {
      wait_us(PERIOD_US_IDLE_SLEEP);
    }
  }
}

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  mbed_error_status_t error = initialize();
  if (error) {
    ERROR("PMIC", "Failed to initialize: 0x%02X", error);
    mbed_die();
  }
  error = run();
  if (error) {
    ERROR("PMIC", "Failed to run: 0x%02X", error);
    mbed_die();
  }
  return MBED_SUCCESS;
}
