//#include <CISConsole.h>
#include "ADCS.h"
#include <mbed.h>


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
  adcs.startThread();
}
