#include <CISConsole.h>
#include <mbed.h>
#include "CISBoard/PinNames.h"
//#include "CISBoard/PeripheralPins.c"
#include "CISBoard/PeripheralNames.h"


/*
#include "ADCSObjects.h"
#include "Configuration.h"
//#include "events/Events.h"
#include "interfaces/CDH.h"
*/
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
  /*mbed_error_status_t result = initialize();
  if (result) {
    ERROR("ADCS", "Failed to initialize: 0x%02X", result);
  }
  result = run();
  if (result) {
    ERROR("ADCS", "Failed to run: 0x%02X", result);
  }
  return MBED_SUCCESS;*/
    printf("Outside Main!\r\n");
    DEBUG("ADCS", "Outside Main!");
    //DigitalOut led1(PinName(5));
    while (true)
    {
        printf("Process Main!\r\n");
        DEBUG("ADCS", "Process Main!");
        //led1 = !led1;
        wait(1);
    }
  //ADCS adcs;
  //adcs.initThread();  
}
