#include <mbed.h>

<<<<<<< HEAD
#include <ADC/AD7689.h>
=======
#include <ADC/AD7291.h>
>>>>>>> ADCS-CONTROL-LOOP
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

  ((AD7689 *)adcEPSs[0])->selfTest();
  ((AD7689 *)adcEPSs[1])->selfTest();
  ((AD7689 *)adcEPSs[2])->selfTest();

  bool inputAOn = true;
  bool inputBOn = true;
  inputSwitching[0]->write(inputAOn);
  inputSwitching[1]->write(inputBOn);
  inputSwitching[2]->write(inputAOn);
  inputSwitching[3]->write(inputBOn);

  bool outputsOn = true;
  bool heatersOn = false;
  for (uint8_t i = 0; i < COUNT_PR_3V3; i++)
    nodesPR3V3[i]->setSwitch(outputsOn);
  for (uint8_t i = 0; i < COUNT_PR_BATT; i++)
    nodesPRBatt[i]->setSwitch(outputsOn);
  for (uint8_t i = 0; i < COUNT_DEPLOY; i++)
    nodesDeployables[i]->setSwitch(outputsOn);
  for (uint8_t i = 0; i < COUNT_BH; i++)
    nodesBatteryHeaters[i]->setSwitch(heatersOn);

  double vbattA = 0.0;
  double vbattB = 0.0;
  double value  = 0.0;
  while (true) {
    statusLED = !statusLED;
    error     = nodeBattInA.updateCurrent();
    if (error) {
      ERROR("Init", "Failed to read in A, 0x%08X", error);
      return error;
    }

    error = nodeBattInB.updateCurrent();
    if (error) {
      ERROR("Init", "Failed to read in B, 0x%08X", error);
      return error;
    }

    error = nodeBattOutA.updateCurrent();
    if (error) {
      ERROR("Init", "Failed to read out A, 0x%08X", error);
      return error;
    }

    error = nodeBattOutB.updateCurrent();
    if (error) {
      ERROR("Init", "Failed to read out B, 0x%08X", error);
      return error;
    }

    error = node3V3OutA.updateCurrent();
    if (error) {
      ERROR("Init", "Failed to read out A, 0x%08X", error);
      return error;
    }

    error = adcEPSs[2]->readVoltage(ADCChannel_t::CM_00, vbattA);
    if (error) {
      ERROR("Init", "Failed to read VbattA, 0x%08X", error);
      return error;
    }
    vbattA =
        2 * vbattA * (nodeBattOutA.getCurrent() - nodeBattInA.getCurrent());

    error = adcEPSs[2]->readVoltage(ADCChannel_t::CM_07, vbattB);
    if (error) {
      ERROR("Init", "Failed to read VbattB, 0x%08X", error);
      return error;
    }
    vbattB =
        2 * vbattB * (nodeBattOutB.getCurrent() - nodeBattInB.getCurrent());

    LOG("Test", "A: %9.5fW\tB: %9.5fW\tA+B: %9.5fW", vbattA, vbattB,
        vbattA + vbattB);
    // double temp;

    // error = thermistorRegA.getTemperature(temp);
    // if (error) {
    //   ERROR("Init", "Failed to read VbattA, 0x%08X", error);
    //   return error;
    // }
    // value = value * 0.875 + temp * 0.125;
    // LOG("Test", "%9.5fA", value);
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

  mbed_error_status_t error = MBED_SUCCESS;
  while (true) {
    now = HAL_GetTick();
    if (now >= nextPeriodicEvent && (nextPeriodicEvent >= PERIOD_MS_PERIODIC ||
                                        now <= PERIOD_MS_PERIODIC)) {
      statusLED = !statusLED;
      error     = eventPeriodic();
      if (error) {
        ERROR("Run", "Failed to perform periodic event: 0x%02X", error);
        return error;
      }
      nextPeriodicEvent = now + PERIOD_MS_PERIODIC;
      // } else if (cdh.hasMessage()) {
      //   error = cdh.processMessage();
      //   if (error) {
      //     ERROR("Run", "Failed to process message from the bus: 0x%02X",
      //     error); return error;
      //   }
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
  mbed_error_status_t error = initialize();
  if (error) {
    ERROR("PMIC", "Failed to initialize: 0x%02X", error);
    return error;
  }
  error = run();
  if (error) {
    ERROR("PMIC", "Failed to run: 0x%02X", error);
    return error;
  }
  return MBED_SUCCESS;
}
