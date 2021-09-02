#include <mbed.h>

#include <ADC/AD7291.h>
#include <ADC/AD7689.h>
#include <CISConsole.h>

#include "Configuration.h"
#include "PMICObjects.h"
#include "events/Events.h"
#include "interfaces/CDH.h"

/**
 * @brief Convert reset reason to string for console printing
 *
 * @param reason
 * @return const char*
 */
const char * resetReason(const reset_reason_t reason) {
  switch (reason) {
    case RESET_REASON_POWER_ON:
      return "Power On";
    case RESET_REASON_PIN_RESET:
      return "Hardware Pin";
    case RESET_REASON_SOFTWARE:
      return "Software Reset";
    case RESET_REASON_WATCHDOG:
      return "Watchdog";
    default:
      return "Other Reason";
  }
}

/**
 * @brief Initializes the all of the subclasses of the PMIC
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t initialize() {
  LOG("Init", "Initialization starting");
  Watchdog & watchdog = Watchdog::get_instance();
  watchdog.start(PERIOD_MS_WATCHDOG);

  mbed_error_status_t error = MBED_SUCCESS;

  reset_reason_t reason = ResetReason::get();
  LOG("Init", "Last reset was due to %s", resetReason(reason));

  // No need for absolute time
  set_time(0);

  error = eventPOST();
  if (error) {
    LOGE("Init", "Failed to perform POST, 0x%08X", error);
    return error;
  }

  double ejectTimer = 0.0;
  // TODO Make VoltageNode and PowerNode classes with gain
  error = adcsEPS[2]->readVoltage(ADCChannel_t::CM_02, ejectTimer);
  if (error) {
    LOGE("Init", "Failed to read eject timer, 0x%08X", error);
    return error;
  }
  ejectTimer *= 2;
  bool firstBoot = ejectTimer < THRES_EJECT_TIMER;
  LOG("Init", "Eject timer: %9.5fV\tFirst boot: %s", ejectTimer,
      firstBoot ? "true" : "false");

  if (firstBoot) {
    error = eventFirstBoot();
    if (error) {
      LOGE("Init", "Failed to perform first bool event: 0x%08X", error);
      return error;
    }
  } else {
    error = eventDeploy();
    if (error) {
      LOGE("Init", "Failed to perform deploy event: 0x%08X", error);
      return error;
    }
  }

  LOG("Init", "Turning C&DH on");
  iNodesPR3V3[NODES_3V3_CDH]->setSwitch(true);

  LOG("Init", "Initialization complete");
  // Init needs to get to this point quicker than the watchdog period
  watchdog.kick();
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

  bool deployWaiting = true;

  Watchdog & watchdog = Watchdog::get_instance();

  mbed_error_status_t error = MBED_SUCCESS;
  while (true) {
    now = HAL_GetTick();
    if (now >= nextPeriodicEvent && (nextPeriodicEvent >= PERIOD_MS_PERIODIC ||
                                        now <= PERIOD_MS_PERIODIC)) {
      statusLED = !statusLED;
      watchdog.kick();
      error = eventPeriodic();
      if (error) {
        LOGE("Run", "Failed to perform periodic event: 0x%08X", error);
        return error;
      }
      nextPeriodicEvent = now + PERIOD_MS_PERIODIC;
    } else if (cdh.hasMessage()) {
      error = cdh.processMessage();
      if (error) {
        LOGE("Run", "Failed to process message from the bus: 0x%08X", error);
        return error;
      }
    } else if (deployWaiting && (uint64_t)time(NULL) > DURATION_S_DEPLOY) {
      error = eventDeploy();
      if (error) {
        LOGE("Init", "Failed to perform deploy event: 0x%08X", error);
        return error;
      }
      deployWaiting = false;
    } else {
      wait_us(PERIOD_US_IDLE_SLEEP);
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
    LOGE("PMIC", "Failed to initialize: 0x%08X", error);
    mbed_die();
  }
  error = run();
  if (error) {
    LOGE("PMIC", "Failed to run: 0x%08X", error);
    mbed_die();
  }
  return MBED_SUCCESS;
}
