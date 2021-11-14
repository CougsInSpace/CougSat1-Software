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

  mbed_error_status_t error = MBED_SUCCESS;
  double              value = 0.0;

  uint8_t testCount   = 0;
  uint8_t passedCount = 0;

  // Step 1: Check internal nodes
  error = internalTemp.get(value);
  ++testCount;
  if (error) {
    LOGE("POST", "Failed to read internal temperature, 0x%08X", error);
  } else if (value < POST_INTERNAL_TEMP_K_LOW ||
             value > POST_INTERNAL_TEMP_K_HIGH) {
    LOGE("POST", "Internal temperature exceeds limits: %9.1fK", value);
  } else {
    LOG("POST", "Internal temperature: %9.1fK", value);
    ++passedCount;
  }

  // Step 2: Test peripherals
  for (int i = 0; i < COUNT_ADC_EPS; ++i) {
    ++testCount;
    error = adcsEPS[i]->selfTest();
    if (error) {
      LOGE("POST", "adcsEPS[%d] failed self test, 0x%08X", i, error);
    } else {
      LOG("POST", "adcsEPS[%d] passed self test", i);
      ++passedCount;
    }
  }

  // Step 3: Check EPS temperatures are not crazy
  for (int i = 0; i < COUNT_THERM_BOARD; ++i) {
    ++testCount;
    error = thermistorsEPS[i]->get(value);
    if (error) {
      LOGE("POST", "thermistorEPSs[%d] failed to get temperature, 0x%08X", i,
          error);
    } else if (value < POST_THERM_BOARD_TEMP_K_LOW ||
               value > POST_THERM_BOARD_TEMP_K_HIGH) {
      LOGE("POST", "thermistorsEPS[%2d] exceeds limits: %9.1fK", i, value);
    } else {
      LOG("POST", "thermistorsEPS[%2d]: %9.1fK", i, value);
      ++passedCount;
    }
  }

  // Step 4: Check EPS voltages are not crazy
  ++testCount;
  error = vNodeBattA.updateAndGet(value);
  if (error) {
    LOGE("POST", "vNodeBattA failed to get voltage, 0x%08X", error);
  } else if (value < POST_BATT_V_LOW || value > POST_BATT_V_HIGH) {
    LOGE("POST", "vNodeBattA exceeds limits: %9.5fV", value);
  } else {
    LOG("POST", "vNodeBattA: %9.5fV", value);
    ++passedCount;
  }
  ++testCount;
  error = vNodeBattB.updateAndGet(value);
  if (error) {
    LOGE("POST", "vNodeBattB failed to get voltage, 0x%08X", error);
  } else if (value < POST_BATT_V_LOW || value > POST_BATT_V_HIGH) {
    LOGE("POST", "vNodeBattB exceeds limits: %9.5fV", value);
  } else {
    LOG("POST", "vNodeBattB: %9.5fV", value);
    ++passedCount;
  }
  ++testCount;
  error = vNode3V3.updateAndGet(value);
  if (error) {
    LOGE("POST", "vNode3V3A failed to get voltage, 0x%08X", error);
  } else if (value < POST_3V3_V_LOW || value > POST_3V3_V_HIGH) {
    LOGE("POST", "vNode3V3A exceeds limits: %9.5fV", value);
  } else {
    LOG("POST", "vNode3V3A: %9.5fV", value);
    ++passedCount;
  }

  // Step 5: Check EPS currents are not crazy
  ++testCount;
  error = iNodeBattInA.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodeBattInA failed to get current, 0x%08X", error);
  } else if (value < POST_BATT_IN_I_LOW || value > POST_BATT_IN_I_HIGH) {
    LOGE("POST", "iNodeBattInA exceeds limits: %9.5fA", value);
  } else {
    LOG("POST", "iNodeBattInA: %9.5fA", value);
    ++passedCount;
  }
  ++testCount;
  error = iNodeBattInB.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodeBattInB failed to get current, 0x%08X", error);
  } else if (value < POST_BATT_IN_I_LOW || value > POST_BATT_IN_I_HIGH) {
    LOGE("POST", "iNodeBattInB exceeds limits: %9.5fA", value);
  } else {
    LOG("POST", "iNodeBattInB: %9.5fA", value);
    ++passedCount;
  }
  ++testCount;
  error = iNodeBattOutA.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodeBattOutA failed to get current, 0x%08X", error);
  } else if (value < POST_BATT_OUT_I_LOW || value > POST_BATT_OUT_I_HIGH) {
    LOGE("POST", "iNodeBattOutA exceeds limits: %9.5fA", value);
  } else {
    LOG("POST", "iNodeBattOutA: %9.5fA", value);
    ++passedCount;
  }
  ++testCount;
  error = iNodeBattOutB.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodeBattOutB failed to get current, 0x%08X", error);
  } else if (value < POST_BATT_OUT_I_LOW || value > POST_BATT_OUT_I_HIGH) {
    LOGE("POST", "iNodeBattOutB exceeds limits: %9.5fA", value);
  } else {
    LOG("POST", "iNodeBattOutB: %9.5fA", value);
    ++passedCount;
  }

  ++testCount;
  error = iNode3V3In.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNode3V3InA failed to get current, 0x%08X", error);
  } else if (value < POST_3V3_IN_I_LOW || value > POST_3V3_IN_I_HIGH) {
    LOGE("POST", "iNode3V3InA exceeds limits: %9.5fA", value);
  } else {
    LOG("POST", "iNode3V3InA: %9.5fA", value);
    ++passedCount;
  }

  ++testCount;
  error = iNode3V3Out.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNode3V3OutA failed to get current, 0x%08X", error);
  } else if (value < POST_3V3_OUT_I_LOW || value > POST_3V3_OUT_I_HIGH) {
    LOGE("POST", "iNode3V3OutA exceeds limits: %9.5fA", value);
  } else {
    LOG("POST", "iNode3V3OutA: %9.5fA", value);
    ++passedCount;
  }
  ++testCount;

  // error = iNode3V3OutB.updateAndGet(value);
  // if (error) {
  //   LOGE("POST", "iNode3V3OutB failed to get current, 0x%08X", error);
  // } else if (value < POST_3V3_OUT_I_LOW || value > POST_3V3_OUT_I_HIGH) {
  //   LOGE("POST", "iNode3V3OutB exceeds limits: %9.5fA", value);
  // } else {
  //   LOG("POST", "iNode3V3OutB: %9.5fA", value);
  //   ++passedCount;
  // }

  ++testCount;
  error = iNodePR3V3_EPS.updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodePR3V3_EPS failed to get current, 0x%08X", error);
  } else if (value < POST_3V3_EPS_I_LOW || value > POST_3V3_EPS_I_HIGH) {
    LOGE("POST", "iNodePR3V3_EPS exceeds limits: %9.5fA", value);
  } else {
    LOG("POST", "iNodePR3V3_EPS: %9.5fA", value);
    ++passedCount;
  }

  for (int i = 0; i < COUNT_PR_3V3; ++i) {
    ++testCount;
    error = iNodesPR3V3[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesPR3V3[%d] failed to get current, 0x%08X", i, error);
    } else if (value < POST_LOAD_I_LOW || value > POST_LOAD_I_HIGH) {
      LOGE("POST", "iNodesPR3V3[%2d] exceeds limits: %9.5fA", i, value);
    } else {
      LOG("POST", "iNodesPR3V3[%2d]: %9.5fA", i, value);
      ++passedCount;
    }
  }

  for (int i = 0; i < COUNT_PR_BATT; ++i) {
    ++testCount;
    error = iNodesPRBatt[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesPRBatt[%d] failed to get current, 0x%08X", i, error);
    } else if (value < POST_LOAD_I_LOW || value > POST_LOAD_I_HIGH) {
      LOGE("POST", "iNodesPRBatt[%2d] exceeds limits: %9.5fA", i, value);
    } else {
      LOG("POST", "iNodesPRBatt[%2d]: %9.5fA", i, value);
      ++passedCount;
    }
  }

  for (int i = 0; i < COUNT_BH; ++i) {
    ++testCount;
    error = iNodesBatteryHeaters[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesBatteryHeaters[%d] failed to get current, 0x%08X", i,
          error);
    } else if (value < POST_LOAD_I_LOW || value > POST_LOAD_I_HIGH) {
      LOGE(
          "POST", "iNodesBatteryHeaters[%2d] exceeds limits: %9.5fA", i, value);
    } else {
      LOG("POST", "iNodesBatteryHeaters[%2d]: %9.5fA", i, value);
      ++passedCount;
    }
  }

  for (int i = 0; i < COUNT_DEPLOY; ++i) {
    ++testCount;
    error = iNodesDeployables[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesDeployables[%d] failed to get current, 0x%08X", i,
          error);
    } else if (value < POST_LOAD_I_LOW || value > POST_LOAD_I_HIGH) {
      LOGE("POST", "iNodesDeployables[%2d] exceeds limits: %9.5fA", i, value);
    } else {
      LOG("POST", "iNodesDeployables[%2d]: %9.5fA", i, value);
      ++passedCount;
    }
  }

  // Step 6: Enable PV monitoring
  ++testCount;
  iNodesPR3V3[NODES_3V3_PV]->setSwitch(true);
  wait_us(10e3);
  error = iNodesPR3V3[NODES_3V3_PV]->updateAndGet(value);
  if (error) {
    LOGE("POST", "iNodePR3V3_PV failed to get current, 0x%08X", error);
  } else if (value < POST_3V3_PV_ON_I_LOW || value > POST_3V3_PV_ON_I_HIGH) {
    LOGE("POST", "iNodePR3V3_PV exceeds limits: %9.5fA", value);
  } else {
    LOG("POST", "iNodePR3V3_PV: %9.5fA", value);
    ++passedCount;
  }
  for (int i = 0; i < COUNT_ADC_PV; ++i) {
    ++testCount;
    error = adcsPV[i]->selfTest();
    if (error) {
      LOGE("POST", "adcsPV[%d] failed self test, 0x%08X", i, error);
    } else {
      LOG("POST", "adcsPV[%d] passed self test", i);
      ++passedCount;
    }
  }

  // Step 7: Check PV temperatures are not crazy
  for (int i = 0; i < COUNT_PV; ++i) {
    ++testCount;
    error = thermistorsMPPT[i]->get(value);
    if (error) {
      LOGE("POST", "thermistors MPPT[%d] failed to get temperature, 0x%08X", i,
          error);
    } else if (value < POST_THERM_MPPT_TEMP_K_LOW ||
               value > POST_THERM_MPPT_TEMP_K_HIGH) {
      LOGE("POST", "thermistorsMPPT[%2d] exceeds limits: %9.1fK", i, value);
    } else {
      LOG("POST", "thermistorsMPPT[%2d]: %9.1fK", i, value);
      ++passedCount;
    }
  }

  // Step 8: Check PV voltages are not crazy
  for (int i = 0; i < COUNT_PV; ++i) {
    ++testCount;
    error = vNodesPV[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "vNodesPV[%d] failed to get voltage, 0x%08X", i, error);
    } else if (value < POST_PV_V_LOW || value > POST_PV_V_HIGH) {
      LOGE("POST", "vNodesPV[%2d] exceeds limits: %9.5fV", i, value);
    } else {
      LOG("POST", "vNodesPV[%2d]: %9.5fV", i, value);
      ++passedCount;
    }
    ++testCount;
    error = vNodesPVIn[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "vNodesPVIn[%d] failed to get voltage, 0x%08X", i, error);
    } else if (value < POST_PV_IN_V_LOW || value > POST_PV_IN_V_HIGH) {
      LOGE("POST", "vNodesPVIn[%2d] exceeds limits: %9.5fV", i, value);
    } else {
      LOG("POST", "vNodesPVIn[%2d]: %9.5fV", i, value);
      ++passedCount;
    }
  }

  // Step 9: Check PV currents are not crazy
  for (int i = 0; i < COUNT_PV; ++i) {
    ++testCount;
    error = iNodesPV[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesPV[%d] failed to get current, 0x%08X", i, error);
    } else if (value < POST_PV_I_LOW || value > POST_PV_I_HIGH) {
      LOGE("POST", "iNodesPV[%2d] exceeds limits: %9.5fA", i, value);
    } else {
      LOG("POST", "iNodesPV[%2d]: %9.5fA", i, value);
      ++passedCount;
    }
    ++testCount;
    error = iNodesPVIn[i]->updateAndGet(value);
    if (error) {
      LOGE("POST", "iNodesPVIn[%d] failed to get current, 0x%08X", i, error);
    } else if (value < POST_PV_IN_I_LOW || value > POST_PV_IN_I_HIGH) {
      LOGE("POST", "iNodesPVIn[%2d] exceeds limits: %9.5fA", i, value);
    } else {
      LOG("POST", "iNodesPVIn[%2d]: %9.5fA", i, value);
      ++passedCount;
    }
  }

  LOG("POST", "POST complete %d/%d passed = %9.5f%%", passedCount, testCount,
      ((double)passedCount / (double)testCount) * 100.0);
  if (passedCount != testCount)
    return MBED_ERROR_INVALID_DATA_DETECTED;
  return MBED_SUCCESS;
}