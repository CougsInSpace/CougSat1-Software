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
  CurrentNode * nodes[], uint8_t nodeCount, double currentToShed) 
{
      double              buf   = 0.0;
      int                 count = 0;
      mbed_error_status_t error = MBED_SUCCESS;

      while (currentToShed > 0.0) 
      {
        // Could get stuck in this loop if all nodes are off but there is more
        // current to shed TODO fix this
        double        maxAggregate = 0.0;
        CurrentNode * maxNode      = nullptr;

        // Find the node with the largest aggregate rank that is enabled
        for (int i = 0; i < nodeCount; ++i)
        {
            buf = nodes[i]->getAggregateRank();
            if (buf > maxAggregate && nodes[i]->getSwitch())
            {
              maxAggregate = buf;
              maxNode      = nodes[i];
            }
            count++; // keep track how many times entered this loop
        }

   
        // Turn off the max node and remove its current
        currentToShed -= maxNode->getCurrent();
        maxNode->setSwitch(false);

        // all nodes off and still looping\
        // return error code
        if (count >= nodeCount && currentToShed > 0.0)
        {
          ERROR("eventPeriodic", 
            "current to shed exceeds total current from loads");
          return MBED_ERROR_FAILED_OPERATION;
        }

      }
  return MBED_SUCCESS;
}

/**
 * @brief Executes the periodic event
 *
 * @return mbed_error_status_t error code
 */
mbed_error_status_t eventPeriodic() {
  
  // Check 3.3V rails first as reducing their load reduces VBatt load
  // Update current for 3.3V nodes
  mbed_error_status_t error = MBED_SUCCESS;
  double buf = 0.0;

  // Update the current by reading the ADC's value and doing some math
  for (uint8_t i = 0; i < COUNT_PR_3V3; ++i) 
  {
    error = nodesPR3V3[i]->updateCurrent();
    
    if (error) {
      ERROR("eventPeriodic",
          "Failed to update current for nodesPR3V3[%d]: 0x%08X", i, error);
      return error;
    }

  }
  error = node3V3OutA.updateCurrent();
  if (error) {
    ERROR("eventPeriodic", "Failed to update current for node3V3OutA: 0x%08X",
        error);
    return error;
  }

  // calculate newest current reading for node3V3OutB
  error = node3V3OutB.updateCurrent();
  if (error) {
    ERROR("eventPeriodic", "Failed to update current for node3V3OutB: 0x%08X",
        error);
    return error;
  }

  // Find the current to shed if regulator > its maximum
  double toShed = 0.0;

  // set buf to the difference of the new current and expected current of 3V3Out
  buf = node3V3OutA.getCurrent() - THRES_CURRENT_REG_MAX;
  if (buf > toShed)
    toShed = buf;

  
  // set buf to the difference of the new current and expected current of 3V3Out
  buf = node3V3OutB.getCurrent() - THRES_CURRENT_REG_MAX;
  if (buf > toShed)
    toShed = buf;
  
  /*****************************************************************************/
  // Check the regulators' temperature
  bool regulatorsOvertemp = false;

  // get the temperature of Regulator A
  error = thermistorRegA.getTemperature(buf);
  if (error) {
    ERROR("eventPeriodic", "Failed to get temp for regulator A: 0x%08X", error);
    return error;
  }

  // check temperature cases: 
  if (buf > (THRES_REG_HOT + THRES_OVERHEATED)) {
    for (uint8_t i = 0; i < COUNT_PR_3V3; ++i) 
      nodesPR3V3[i]->setSwitch(false);
  } else if (buf > THRES_REG_HOT) 
  {
    regulatorsOvertemp = true;
  }

  error = thermistorRegB.getTemperature(buf);
  if (error) {
    ERROR("eventPeriodic", "Failed to get temp for regulator B: 0x%08X", error);
    return error;
  }

  if (buf > (THRES_REG_HOT + THRES_OVERHEATED)) 
  {
    //Switch off all loads
    for (uint8_t i = 0; i < COUNT_PR_3V3; ++i) 
      nodesPR3V3[i]->setSwitch(false);
  } 
  else if (buf > THRES_REG_HOT) {
    regulatorsOvertemp = true;
  }

  if (toShed < 0 && regulatorsOvertemp) {
    // If overtemp, reduce load by 25%
    toShed = node3V3OutA.getCurrent() * 0.25 + node3V3OutB.getCurrent() * 0.25;
  }


  error = shedLoad(nodesPR3V3, COUNT_PR_3V3, toShed);
  if (error) {
    ERROR("eventPeriodic", "Failed to shed load from 3.3V: 0x%08X", error);
    return error;
  }

  /*******************************************************/
  // Check battery rails
  // Update current readings for battery rails
  for (uint8_t i = 0; i < COUNT_PR_3V3; ++i) {
    error = nodesPRBatt[i]->updateCurrent();
    if (error) {
      ERROR("eventPeriodic",
          "Failed to update current for nodesPRBatt[%d]: 0x%08X", i, error);
      return error;
    }
  }

  error = nodeBattOutA.updateCurrent();
  if (error) {
    ERROR("eventPeriodic", "Failed to update current for nodeBattOutA: 0x%08X",
        error);
    return error;
  }

  error = nodeBattOutB.updateCurrent();
  if (error) {
    ERROR("eventPeriodic", "Failed to update current for nodeBattOutB: 0x%08X",
        error);
    return error;
  }

  // Find the current to shed if regulator > its maximum
  toShed = 0.0;

  buf = nodeBattOutA.getCurrent() - THRES_CURRENT_BATT_MAX;
  if (buf > toShed)
    toShed = buf;

  buf = nodeBattOutB.getCurrent() - THRES_CURRENT_BATT_MAX;
  if (buf > toShed)
    toShed = buf;

  // Check the battery temperature
  bool batteriesOvertemp = false;

  error = thermistorBattA.getTemperature(buf);
  if (error) {
    ERROR("eventPeriodic", "Failed to get temp for battery A: 0x%08X", error);
    return error;
  }

  if (buf > (THRES_BATT_HOT + THRES_OVERHEATED)) {
    // TODO: turn off all loads
    for (uint8_t i = 0; i < COUNT_PR_BATT; ++i) 
      nodesPRBatt[i]->setSwitch(false);
  } else if (buf > THRES_BATT_HOT) {
    batteriesOvertemp = true;
  } else if (buf < THRES_BATT_COLD) {
    // Turn on heater for cold battery
    nodesBatteryHeaters[0]->setSwitch(true);
  }

  error = thermistorBattB.getTemperature(buf);
  if (error) {
    ERROR("eventPeriodic", "Failed to get temp for battery B: 0x%08X", error);
    return error;
  }

  if (buf > (THRES_BATT_HOT + THRES_OVERHEATED)) {
    // TODO: turn off all loads:
    for (uint8_t i = 0; i < COUNT_PR_BATT; ++i) 
      nodesPRBatt[i]->setSwitch(false);
  } else if (buf > THRES_BATT_HOT) {
    batteriesOvertemp = true;
  } else if (buf < THRES_BATT_COLD) {
    // Turn on heater for cold battery
    nodesBatteryHeaters[1]->setSwitch(true);
  }

  if (toShed < 0 && batteriesOvertemp) {
    // If overtemp, shed 25%
    toShed = nodeBattOutA.getCurrent() * 0.25 + nodeBattOutB.getCurrent() * 0.25;
  }
  
  error = shedLoad(nodesPRBatt, COUNT_PR_BATT, toShed);
  if (error) {
    ERROR("eventPeriodic", "Failed to shed load from VBatt: 0x%08X", error);
    return error;
  }

  return MBED_SUCCESS;
}