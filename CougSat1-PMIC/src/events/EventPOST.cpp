#include "Events.h"
#include "PMICObjects.h"

#include <CISConsole.h>

/**
 * @brief Perform Power-On Self-Test (POST)
 *
 * For the most part, this tests if the returned data is realistic for the
 * sensor type not the application
 *
 * @return mbed_error_status_t error code
 */
mbed_error_status_t eventPOST() {
  LOG("POST", "POST starting");

  mbed_error_status_t error     = MBED_SUCCESS;
  mbed_error_status_t lastError = MBED_SUCCESS;
  double              value     = 0.0;

  // Step 1: Check internal nodes
  error = internalTemp.get(value);
  if (error) {
    LOGE("POST", "Failed to read internal temperature, 0x%08X", error);
    // TODO Determine if this (and all errors) should be ignored or not.
    return error;
  }
  if (value < POST_INTERNAL_TEMP_K_LOW || value > POST_INTERNAL_TEMP_K_HIGH) {
    LOGE("POST", "Internal temperature exceeds limits: %9.1fK", value);
    return MBED_ERROR_INVALID_DATA_DETECTED;
  }
  LOG("POST", "Internal temperature okay: %9.1fK", value);

  // Step 2: Test peripherals
  lastError = MBED_SUCCESS;
  for (int i = 0; i < COUNT_ADC_EPS; ++i) {
    error = adcsEPS[i]->selfTest();
    if (error) {
      LOGE("POST", "adcsEPS[%d] failed self test, 0x%08X", i, error);
      lastError = error;
    } else {
      LOG("POST", "adcsEPS[%d] passed self test", i);
    }
  }
  if (lastError != MBED_SUCCESS)
    return lastError;

  // Step 3: Check EPS temperatures are not crazy
  lastError = MBED_SUCCESS;
  for (int i = 0; i < COUNT_THERM_BOARD; ++i) {
    error = thermistorsEPS[i]->get(value);
    if (error) {
      LOGE("POST", "thermistorEPSs[%d] failed to get temperature, 0x%08X", i,
          error);
      lastError = error;
    } else if (value < POST_THERM_BOARD_TEMP_K_LOW ||
               value > POST_THERM_BOARD_TEMP_K_HIGH) {
      LOGE("POST", "thermistorsEPS[%2d] exceeds limits: %9.1fK", i, value);
      lastError = MBED_ERROR_INVALID_DATA_DETECTED;
    } else {
      LOG("POST", "thermistorsEPS[%2d]: %9.1fK", i, value);
    }
  }
  if (lastError != MBED_SUCCESS)
    return lastError;

  // Step 4: Check EPS voltages are not crazy
  lastError = MBED_SUCCESS;

  error = vNodeBattA.updateAndGet(value);
  if (error) {
    LOGE("POST", "vNodeBattA failed to get voltage, 0x%08X", error);
    lastError = error;
  } else if (value < POST_BATT_V_LOW || value > POST_BATT_V_HIGH) {
    LOGE("POST", "vNodeBattA exceeds limits: %9.5fV", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "vNodeBattA: %9.5fV", value);
  }
  error = vNodeBattB.updateAndGet(value);
  if (error) {
    LOGE("POST", "vNodeBattB failed to get voltage, 0x%08X", error);
    lastError = error;
  } else if (value < POST_BATT_V_LOW || value > POST_BATT_V_HIGH) {
    LOGE("POST", "vNodeBattB exceeds limits: %9.5fV", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "vNodeBattB: %9.5fV", value);
  }
  error = vNode3V3A.updateAndGet(value);
  if (error) {
    LOGE("POST", "vNode3V3A failed to get voltage, 0x%08X", error);
    lastError = error;
  } else if (value < POST_3V3_V_LOW || value > POST_3V3_V_HIGH) {
    LOGE("POST", "vNode3V3A exceeds limits: %9.5fV", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "vNode3V3A: %9.5fV", value);
  }
  error = vNode3V3B.updateAndGet(value);
  if (error) {
    LOGE("POST", "vNode3V3B failed to get voltage, 0x%08X", error);
    lastError = error;
  } else if (value < POST_3V3_V_LOW || value > POST_3V3_V_HIGH) {
    LOGE("POST", "vNode3V3B exceeds limits: %9.5fV", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "vNode3V3B: %9.5fV", value);
  }
  if (lastError != MBED_SUCCESS)
    return lastError;

  // Step 5: Check EPS currents are not crazy
  lastError = MBED_SUCCESS;

  error = iNodeBattInA.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodeBattInA failed to get current, 0x%08X", error);
    lastError = error;
  } else if (value < POST_BATT_IN_I_LOW || value > POST_BATT_IN_I_HIGH) {
    LOGE("POST", "iNodeBattInA exceeds limits: %9.5fA", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "iNodeBattInA: %9.5fA", value);
  }
  error = iNodeBattInB.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodeBattInB failed to get current, 0x%08X", error);
    lastError = error;
  } else if (value < POST_BATT_IN_I_LOW || value > POST_BATT_IN_I_HIGH) {
    LOGE("POST", "iNodeBattInB exceeds limits: %9.5fA", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "iNodeBattInB: %9.5fA", value);
  }
  error = iNodeBattOutA.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodeBattOutA failed to get current, 0x%08X", error);
    lastError = error;
  } else if (value < POST_BATT_OUT_I_LOW || value > POST_BATT_OUT_I_HIGH) {
    LOGE("POST", "iNodeBattOutA exceeds limits: %9.5fA", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "iNodeBattOutA: %9.5fA", value);
  }
  error = iNodeBattOutB.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodeBattOutB failed to get current, 0x%08X", error);
    lastError = error;
  } else if (value < POST_BATT_OUT_I_LOW || value > POST_BATT_OUT_I_HIGH) {
    LOGE("POST", "iNodeBattOutB exceeds limits: %9.5fA", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "iNodeBattOutB: %9.5fA", value);
  }

  error = iNode3V3InA.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNode3V3InA failed to get current, 0x%08X", error);
    lastError = error;
  } else if (value < POST_3V3_IN_I_LOW || value > POST_3V3_IN_I_HIGH) {
    LOGE("POST", "iNode3V3InA exceeds limits: %9.5fA", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "iNode3V3InA: %9.5fA", value);
  }
  error = iNode3V3InB.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNode3V3InB failed to get current, 0x%08X", error);
    lastError = error;
  } else if (value < POST_3V3_IN_I_LOW || value > POST_3V3_IN_I_HIGH) {
    LOGE("POST", "iNode3V3InB exceeds limits: %9.5fA", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "iNode3V3InB: %9.5fA", value);
  }
  error = iNode3V3OutA.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNode3V3OutA failed to get current, 0x%08X", error);
    lastError = error;
  } else if (value < POST_3V3_OUT_I_LOW || value > POST_3V3_OUT_I_HIGH) {
    LOGE("POST", "iNode3V3OutA exceeds limits: %9.5fA", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "iNode3V3OutA: %9.5fA", value);
  }
  error = iNode3V3OutB.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNode3V3OutB failed to get current, 0x%08X", error);
    lastError = error;
  } else if (value < POST_3V3_OUT_I_LOW || value > POST_3V3_OUT_I_HIGH) {
    LOGE("POST", "iNode3V3OutB exceeds limits: %9.5fA", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "iNode3V3OutB: %9.5fA", value);
  }

  error = iNodePR3V3_EPS.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodePR3V3_EPS failed to get current, 0x%08X", error);
    lastError = error;
  } else if (value < POST_3V3_EPS_I_LOW || value > POST_3V3_EPS_I_HIGH) {
    LOGE("POST", "iNodePR3V3_EPS exceeds limits: %9.5fA", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "iNodePR3V3_EPS: %9.5fA", value);
  }

  for (int i = 0; i < COUNT_PR_3V3; ++i) {
    error = iNodesPR3V3[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesPR3V3[%d] failed to get current, 0x%08X", i, error);
      lastError = error;
    } else if (value < POST_LOAD_I_LOW || value > POST_LOAD_I_HIGH) {
      LOGE("POST", "iNodesPR3V3[%2d] exceeds limits: %9.5fA", i, value);
      lastError = MBED_ERROR_INVALID_DATA_DETECTED;
    } else {
      LOG("POST", "iNodesPR3V3[%2d]: %9.5fA", i, value);
    }
  }

  for (int i = 0; i < COUNT_PR_BATT; ++i) {
    error = iNodesPRBatt[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesPRBatt[%d] failed to get current, 0x%08X", i, error);
      lastError = error;
    } else if (value < POST_LOAD_I_LOW || value > POST_LOAD_I_HIGH) {
      LOGE("POST", "iNodesPRBatt[%2d] exceeds limits: %9.5fA", i, value);
      lastError = MBED_ERROR_INVALID_DATA_DETECTED;
    } else {
      LOG("POST", "iNodesPRBatt[%2d]: %9.5fA", i, value);
    }
  }

  for (int i = 0; i < COUNT_BH; ++i) {
    error = iNodesBatteryHeaters[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesBatteryHeaters[%d] failed to get current, 0x%08X", i,
          error);
      lastError = error;
    } else if (value < POST_LOAD_I_LOW || value > POST_LOAD_I_HIGH) {
      LOGE(
          "POST", "iNodesBatteryHeaters[%2d] exceeds limits: %9.5fA", i, value);
      lastError = MBED_ERROR_INVALID_DATA_DETECTED;
    } else {
      LOG("POST", "iNodesBatteryHeaters[%2d]: %9.5fA", i, value);
    }
  }

  for (int i = 0; i < COUNT_DEPLOY; ++i) {
    error = iNodesDeployables[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesDeployables[%d] failed to get current, 0x%08X", i,
          error);
      lastError = error;
    } else if (value < POST_LOAD_I_LOW || value > POST_LOAD_I_HIGH) {
      LOGE("POST", "iNodesDeployables[%2d] exceeds limits: %9.5fA", i, value);
      lastError = MBED_ERROR_INVALID_DATA_DETECTED;
    } else {
      LOG("POST", "iNodesDeployables[%2d]: %9.5fA", i, value);
    }
  }

  if (lastError != MBED_SUCCESS)
    return lastError;

  // Step 6: Enable PV monitoring
  lastError = MBED_SUCCESS;

  iNodesPR3V3[NODES_3V3_PV]->setSwitch(true);
  error = iNodesPR3V3[NODES_3V3_PV]->updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodePR3V3_PV failed to get current, 0x%08X", error);
    lastError = error;
  } else if (value < POST_3V3_PV_ON_I_LOW || value > POST_3V3_PV_ON_I_HIGH) {
    LOGE("POST", "iNodePR3V3_PV exceeds limits: %9.5fA", value);
    lastError = MBED_ERROR_INVALID_DATA_DETECTED;
  } else {
    LOG("POST", "iNodePR3V3_PV: %9.5fA", value);
  }
  for (int i = 0; i < COUNT_ADC_PV; ++i) {
    error = adcsPV[i]->selfTest();
    if (error) {
      LOGE("POST", "adcsPV[%d] failed self test, 0x%08X", i, error);
      lastError = error;
    } else {
      LOG("POST", "adcsPV[%d] passed self test", i);
    }
  }
  if (lastError != MBED_SUCCESS)
    return lastError;

  // Step 7: Check PV temperatures are not crazy
  lastError = MBED_SUCCESS;
  for (int i = 0; i < COUNT_PV; ++i) {
    error = thermistorsMPPT[i]->get(value);
    if (error) {
      LOGE("POST", "thermistorsMPPT[%d] failed to get temperature, 0x%08X", i,
          error);
      lastError = error;
    } else if (value < POST_THERM_MPPT_TEMP_K_LOW ||
               value > POST_THERM_MPPT_TEMP_K_HIGH) {
      LOGE("POST", "thermistorsMPPT[%2d] exceeds limits: %9.1fK", i, value);
      lastError = MBED_ERROR_INVALID_DATA_DETECTED;
    } else {
      LOG("POST", "thermistorsMPPT[%2d]: %9.1fK", i, value);
    }
  }
  if (lastError != MBED_SUCCESS)
    return lastError;

  // Step 8: Check PV voltages are not crazy
  lastError = MBED_SUCCESS;
  for (int i = 0; i < COUNT_PV; ++i) {
    error = vNodesPV[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "vNodesPV[%d] failed to get voltage, 0x%08X", i, error);
      lastError = error;
    } else if (value < POST_PV_V_LOW || value > POST_PV_V_HIGH) {
      LOGE("POST", "vNodesPV[%2d] exceeds limits: %9.5fV", i, value);
      lastError = MBED_ERROR_INVALID_DATA_DETECTED;
    } else {
      LOG("POST", "vNodesPV[%2d]: %9.5fV", i, value);
    }
    error = vNodesPVIn[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "vNodesPVIn[%d] failed to get voltage, 0x%08X", i, error);
      lastError = error;
    } else if (value < POST_PV_IN_V_LOW || value > POST_PV_IN_V_HIGH) {
      LOGE("POST", "vNodesPVIn[%2d] exceeds limits: %9.5fV", i, value);
      lastError = MBED_ERROR_INVALID_DATA_DETECTED;
    } else {
      LOG("POST", "vNodesPVIn[%2d]: %9.5fV", i, value);
    }
  }
  if (lastError != MBED_SUCCESS)
    return lastError;

  // Step 9: Check PV currents are not crazy
  lastError = MBED_SUCCESS;
  for (int i = 0; i < COUNT_PV; ++i) {
    error = iNodesPV[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesPV[%d] failed to get current, 0x%08X", i, error);
      lastError = error;
    } else if (value < POST_PV_I_LOW || value > POST_PV_I_HIGH) {
      LOGE("POST", "iNodesPV[%2d] exceeds limits: %9.5fA", i, value);
      lastError = MBED_ERROR_INVALID_DATA_DETECTED;
    } else {
      LOG("POST", "iNodesPV[%2d]: %9.5fA", i, value);
    }
    error = iNodesPVIn[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesPVIn[%d] failed to get current, 0x%08X", i, error);
      lastError = error;
    } else if (value < POST_PV_IN_I_LOW || value > POST_PV_IN_I_HIGH) {
      LOGE("POST", "iNodesPVIn[%2d] exceeds limits: %9.5fA", i, value);
      lastError = MBED_ERROR_INVALID_DATA_DETECTED;
    } else {
      LOG("POST", "iNodesPVIn[%2d]: %9.5fA", i, value);
    }
  }
  if (lastError != MBED_SUCCESS)
    return lastError;

  LOG("POST", "POST complete");
  return MBED_SUCCESS;
}