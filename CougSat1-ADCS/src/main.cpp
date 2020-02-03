//#include <CISConsole.h>
#include <mbed.h>
#include "ADCS.h"

/**
 * @brief Initializes the all of the subclasses of the ADCS
 *
 * @return mbed_error_status_t
 */
//  mbed_error_status_t run() {
//  uint32_t now               = HAL_GetTick();
//  uint32_t nextControlLoop   = now + PERIOD_MS_CONTROL_LOOP;
//  uint32_t nextPeriodicEvent = now + PERIOD_MS_PERIODIC;

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  ADCS adcs;
  adcs.startThread();  
}
