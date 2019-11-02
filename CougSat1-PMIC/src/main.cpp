#include <mbed.h>

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

  statusLED = 1;

  error = ((AD7689 *)adcEPSs[2])->selfTest();
  if (error) {
    ERROR("Init", "ADC did not pass self test, 0x%08X", error);
    return error;
  }

  double buf = 0.0;
  while (true) {
    error = adcEPSs[2]->readTemp(buf);
    if (error) {
      ERROR("Init", "ADC failed to read its temp, 0x%08X", error);
      return error;
    }
    LOG("Init", "%7.5f", buf);
    error = adcEPSs[5]->readTemp(buf);
    if (error) {
      ERROR("Init", "ADC failed to read its temp, 0x%08X", error);
      return error;
    }
    LOG("Init", "%7.5f", buf);
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
