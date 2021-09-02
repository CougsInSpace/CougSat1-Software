#include "Events.h"
#include "PMICObjects.h"

#include <CISConsole.h>

/**
 * @brief Initializes hardware and timers into first boot configuration
 *
 * @param i Index of deployable to deploy
 * @param force True will ignore deploy sense, False will check and only deploy
 * those that are not already deployed
 * @return mbed_error_status_t error code
 */
mbed_error_status_t eventDeploy(int i, bool force) {
  mbed_error_status_t error = MBED_SUCCESS;
  double              value = 0.0;

  bool doDeploy = force;
  error         = vSenseDeployables[i]->updateAndGet(value);
  if (error) {
    LOGE("DEPLOY", "Failed to read vSenseDeployables[%2d], 0x%08X", i, error);
    // Assume this means not deployed
    doDeploy = true;
  } else {
    LOGD("DEPLOY", "vSenseDeployables[%2d]: %9.5fV", i, value);
    if (value < THRES_DEPLOY_SENSE_LOW || value > THRES_DEPLOY_SENSE_HIGH) {
      doDeploy = true;
    }
  }
  if (!doDeploy) {
    LOG("DEPLOY", "Deployables[%2d] already deployed", i);
  }
  LOG("DEPLOY", "Deployables[%2d] deploying", i);

  // Dump power into deployable to accumulate 5J of energy
  // Or stop after 10 seconds
  constexpr double TARGET_ENERGY_J = 5.0;
  constexpr int    TIMEOUT_MS      = 10 * 1000;

  double   energy   = 0.0;
  double   voltageA = 0.0;
  double   voltageB = 0.0;
  double   current  = 0.0;
  double   power    = 0.0;
  uint32_t start    = HAL_GetTick();

  iNodesDeployables[i]->setSwitch(true);
  uint32_t lastUpdate = start;
  uint32_t now        = start;
  while (energy < TARGET_ENERGY_J && (HAL_GetTick() - start) < TIMEOUT_MS) {
    // TODO replace with PowerNode
    error = iNodesDeployables[i]->updateAndGet(current);
    if (error) {
      LOGE("DEPLOY", "Failed to read iNodesDeployables[%2d], 0x%08X", i, error);
      iNodesDeployables[i]->setSwitch(false);
    }
    error = vNodeBattA.updateAndGet(voltageA);
    if (error) {
      LOGE("DEPLOY", "Failed to read vNodeBattA, 0x%08X", error);
      iNodesDeployables[i]->setSwitch(false);
    }
    error = vNodeBattB.updateAndGet(voltageB);
    if (error) {
      LOGE("DEPLOY", "Failed to read vNodeBattB, 0x%08X", error);
      iNodesDeployables[i]->setSwitch(false);
    }

    now   = HAL_GetTick();
    power = max(voltageA, voltageB) * current;
    energy += power * static_cast<double>(now - lastUpdate) / 1000.0;

    lastUpdate = now;
  }
  iNodesDeployables[i]->setSwitch(false);

  error = vSenseDeployables[i]->updateAndGet(value);
  if (error) {
    LOGE("DEPLOY", "Failed to read vSenseDeployables[%2d], 0x%08X", i, error);
  } else {
    LOGD("DEPLOY", "vSenseDeployables[%2d]: %9.5fV", i, value);
    if (value < THRES_DEPLOY_SENSE_LOW || value > THRES_DEPLOY_SENSE_HIGH) {
      LOGE("DEPLOY", "Deployables[%2d] deployment failed", i);
      return MBED_ERROR_FAILED_OPERATION;
    } else {
      LOG("DEPLOY", "Deployables[%2d] deployment succeeded", i);
    }
  }

  return MBED_SUCCESS;
}