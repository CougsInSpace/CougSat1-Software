#include <CISConsole.h>
#include <mbed.h>

#include "Configuration.h"
#include "IHUObjects.h"
#include "events/Events.h"
#include <cstdio>
#include "SDBlockDevice.h"
#include <FATFileSystem.h>

SDBlockDevice sd(D11, D12, D13, D10);
FATFileSystem fs("sd");


/**
 * @brief Initializes the all of the subclasses of the IHU
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t initialize() {
  LOG("Init", "Initialization starting");
  mbed_error_status_t result = MBED_SUCCESS;
  
  // file system initialize
  sd.init();
  fs.mount(&sd);
  
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
