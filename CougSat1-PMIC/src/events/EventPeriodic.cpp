#include "Events.h"
#include <CISConsole.h>
#include <PMICObjects.h>

/**
 * @brief Shed current from the nodes based on their rank
 * Node with the largest (current * rank) is first to be shed
 *
 * @param nodes to shed from
 * @param nodeCount of the nodes
 * @param currentToShed in amps
 * @return mbed_error_status_t
 */
mbed_error_status_t shedLoad(
    CurrentNode * nodes[], uint8_t nodeCount, double currentToShed) {
  double              buf   = 0.0;
  mbed_error_status_t error = MBED_SUCCESS;

  while (currentToShed > 0.0) {
    // Could get stuck in this loop if all nodes are off but there is more
    double        maxAggregate = 0.0;
    CurrentNode * maxNode      = nullptr;
    uint8_t       count        = 0;

    // Find the node with the largest aggregate rank that is enabled
    for (int i = 0; i < nodeCount; ++i) {
      buf = nodes[i]->getAggregateRank();
      if (nodes[i]->getSwitch()) {
        count++;
        if (buf > maxAggregate) {
          maxAggregate = buf;
          maxNode      = nodes[i];
        }
      }
    }
    // Turn off the max node and remove its current
    currentToShed -= maxNode->get();
    maxNode->setSwitch(false);

    if (count == 0)
      return MBED_ERROR_OPERATION_ABORTED;
  }
  return error;
}

/**
 * @brief Executes the periodic event
 *
 * @return mbed_error_status_t error code
 */
mbed_error_status_t eventPeriodic() {
  // TODO update for EPS V3.1
  return MBED_SUCCESS;

  // Check 3.3V rails first as reducing their load reduces VBatt load
  // Update current for 3.3V nodes
  mbed_error_status_t error = MBED_SUCCESS;
  double              buf   = 0.0;

  // Update the current by reading the ADC's value and doing some math
  for (uint8_t i = 0; i < COUNT_PR_3V3; ++i) {
    error = iNodesPR3V3[i]->update();

    if (error) {
      LOGE("eventPeriodic",
          "Failed to update current for nodesPR3V3[%d]: 0x%08X", i, error);
      return error;
    }
  }
  error = iNode3V3Out.update();
  if (error) {
    LOGE("eventPeriodic", "Failed to update current for node3V3OutA: 0x%08X",
        error);
    return error;
  }


  // Find the current to shed if regulator > its maximum
  double toShed = 0.0;

  buf = iNode3V3Out.get() - THRES_CURRENT_REG_MAX;
  if (buf > toShed)
    toShed = buf;
  
  // Check the regulators' temperature
  bool regulatorsOvertemp = false;

  error = thermistorRegA.get(buf);
  if (error) {
    LOGE("eventPeriodic", "Failed to get temp for regulator A: 0x%08X", error);
    return error;
  }

  if (buf > (THRES_REG_HOT + THRES_OVERHEATED)) {
    for (uint8_t i = 0; i < COUNT_PR_3V3; ++i)
      iNodesPR3V3[i]->setSwitch(false);
  } else if (buf > THRES_REG_HOT) {
    regulatorsOvertemp = true;
  }

  error = thermistorRegB.get(buf);
  if (error) {
    LOGE("eventPeriodic", "Failed to get temp for regulator B: 0x%08X", error);
    return error;
  }

  if (buf > (THRES_REG_HOT + THRES_OVERHEATED)) {
    // Switch off all loads
    for (uint8_t i = 0; i < COUNT_PR_3V3; ++i)
      iNodesPR3V3[i]->setSwitch(false);
  } else if (buf > THRES_REG_HOT) {
    regulatorsOvertemp = true;
  }

  if (toShed < 0 && regulatorsOvertemp) {
    // If overtemp, reduce load by 25%
    toShed = iNode3V3Out.get() * 0.25;
  }

  error = shedLoad(iNodesPR3V3, COUNT_PR_3V3, toShed);
  if (error) {
    LOGE("eventPeriodic", "Failed to shed load from 3.3V: 0x%08X", error);
    return error;
  }

  // Check battery rails
  // Update current readings for battery rails
  for (uint8_t i = 0; i < COUNT_PR_3V3; ++i) {
    error = iNodesPRBatt[i]->update();
    if (error) {
      LOGE("eventPeriodic",
          "Failed to update current for nodesPRBatt[%d]: 0x%08X", i, error);
      return error;
    }
  }

  error = iNodeBattOutA.update();
  if (error) {
    LOGE("eventPeriodic", "Failed to update current for nodeBattOutA: 0x%08X",
        error);
    return error;
  }

  error = iNodeBattOutB.update();
  if (error) {
    LOGE("eventPeriodic", "Failed to update current for nodeBattOutB: 0x%08X",
        error);
    return error;
  }

  // Find the current to shed if regulator > its maximum
  toShed = 0.0;

  buf = iNodeBattOutA.get() - THRES_CURRENT_BATT_MAX;
  if (buf > toShed)
    toShed = buf;

  buf = iNodeBattOutB.get() - THRES_CURRENT_BATT_MAX;
  if (buf > toShed)
    toShed = buf;

  // Check the battery temperature
  bool batteriesOvertemp = false;

  error = thermistorBattA.get(buf);
  if (error) {
    LOGE("eventPeriodic", "Failed to get temp for battery A: 0x%08X", error);
    return error;
  }

  if (buf > (THRES_BATT_HOT + THRES_OVERHEATED)) {
    // turn off all loads
    for (uint8_t i = 0; i < COUNT_PR_BATT; ++i)
      iNodesPRBatt[i]->setSwitch(false);
  } else if (buf > THRES_BATT_HOT) {
    batteriesOvertemp = true;
  } else if (buf < THRES_BATT_COLD) {
    // Turn on heater for cold battery
    iNodesBatteryHeaters[0]->setSwitch(true);
  }

  error = thermistorBattB.get(buf);
  if (error) {
    LOGE("eventPeriodic", "Failed to get temp for battery B: 0x%08X", error);
    return error;
  }

  if (buf > (THRES_BATT_HOT + THRES_OVERHEATED)) {
    // turn off all loads:
    for (uint8_t i = 0; i < COUNT_PR_BATT; ++i)
      iNodesPRBatt[i]->setSwitch(false);
  } else if (buf > THRES_BATT_HOT) {
    batteriesOvertemp = true;
  } else if (buf < THRES_BATT_COLD) {
    // Turn on heater for cold battery
    iNodesBatteryHeaters[1]->setSwitch(true);
  }

  if (toShed < 0 && batteriesOvertemp) {
    // If overtemp, shed 25%
    toShed = iNodeBattOutA.get() * 0.25;
  }

  error = shedLoad(iNodesPRBatt, COUNT_PR_BATT, toShed);
  if (error) {
    LOGE("eventPeriodic", "Failed to shed load from VBatt: 0x%08X", error);
    return error;
  }

  return MBED_SUCCESS;
}