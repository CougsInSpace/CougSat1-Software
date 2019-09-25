#include "Events.h"
#include <CISConsole.h>

/**
 * Function: Load_shedding()
 * Input: long current_to_shed
 * Output: mbed_error_status_t
 * Description:
 *    This function will accept a long type variable that will contain
 *    an current value called current_to_shed. This current value will 
 *    be used to decide when to stop shedding payloads. Each payload 
 *    has an expected current, so for each payload that is shed,
 *    that payloads expected current will be subtracted from the 
 *    current_to_shed value. Once the current_to_shed value reaches
 *    zero, exit the program. 
 * 
 * */
mbed_error_status_t Load_shedding(long current_to_shed)
{
  

  
  return 0;
}



/**
 * @brief Executes the periodic event
 *
 * @return mbed_error_status_t error code
 */
mbed_error_status_t eventPeriodic() 
{
    // Check each module or peripheral that needs to be monitored
    // if any of the measured values is out of the norm, call load_shedding()
    // function and pass in a current amount to shed

    





  return MBED_ERROR_UNSUPPORTED;
}