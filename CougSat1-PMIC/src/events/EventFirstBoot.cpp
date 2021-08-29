#include "Events.h"
#include "PMICObjects.h"

#include <CISConsole.h>

/**
 * @brief Initializes hardware and timers into first boot configuration
 *
 * @return mbed_error_status_t error code
 */
mbed_error_status_t eventFirstBoot() {
  // eventDeploy handled in loop

  // Turn off Comms for now
  nodesPR3V3[NODES_3V3_COMMS]->setSwitch(false);
  nodesPRBatt[NODES_BATT_COMMS]->setSwitch(false);

  return MBED_SUCCESS;
}