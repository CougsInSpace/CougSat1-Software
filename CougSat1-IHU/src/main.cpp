#include <CISConsole.h>
#include <mbed.h>

#include "Configuration.h"
#include "IHUObjects.h"
#include "events/Events.h"

/**
 * @brief Initializes the all of the subclasses of the IHU
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t initialize() {
  LOG("Init", "Initialization starting");
  mbed_error_status_t result = MBED_SUCCESS;

  LOG("Init", "Initialization complete");
  return result;
}

/**
 * @brief Exectutes the main loop of the IHU
 *
 * @return mbed_error_status_t error code
 */
mbed_error_status_t run() {
  if (queue.call_every(PERIOD_MS_PERIODIC, eventPeriodic) == 0)
    return MBED_ERROR_OUT_OF_MEMORY;

  queue.dispatch_forever();
  return MBED_SUCCESS;
}

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  mbed_error_status_t result = initialize();
  if (result) {
    ERROR("IHU", "Failed to initialize: 0x%02X", result);
  }
  result = run();
  if (result) {
    ERROR("IHU", "Failed to initialize: 0x%02X", result);
  }
  return MBED_SUCCESS;
}
