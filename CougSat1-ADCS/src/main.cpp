#include "../../CISLibrary/drivers/GPS/Venus838FLPx.h"
#include "../tools/CISConsole.h"
#include "Configuration.h"
#include "interfaces/CDH.h"
#include <CISConsole.h>
#include <mbed.h>
#include <rtos.h>

//#include "../../CISLibrary/tools/CISConsole.h"
//#include "ADCS.h"

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
  LOG("ADCS", "Start");
  // ADCS adcs;
  // adcs.startThread();
  RawSerial           gpsSerial = RawSerial(PA_2, USBRX, 9600);
  Venus838FLPx        gps1(gpsSerial, PinName(PA_8), PinName(PA_11));
  GPSData_t           gpsData;
  mbed_error_status_t error = 0;
  DigitalOut led(D4);
  led = 0;

  while (true) {
    LOG("ADCS", "Main");
    printf("put someting on serial\n");
    
    error = gps1.read(gpsData);
    if (!error) {
      led = !led;
      LOG("GPS",
          "Latitude: %f\n   Longitude: %f\n Altitude: %f\n  Ground Speed: %f  "
          "\n "
          "Current Time: %d",
          gpsData.latitude, gpsData.longitude, gpsData.altitude,
          gpsData.groundSpeed, gpsData.time);
    }
    wait(.5);
  }
}
