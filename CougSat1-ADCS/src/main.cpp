#include <CISConsole.h>
#include <mbed.h>
#include "ADCSObjects.h"
#include "Configuration.h"
#include "interfaces/CDH.h"

/**
 * @brief Initializes the all of the subclasses of the ADCS
 *
 * @return mbed_error_status_t
 */

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  ADCS adcs;
  adcs.initThread();  
}
