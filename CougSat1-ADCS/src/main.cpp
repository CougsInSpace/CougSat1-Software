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
  DigitalOut led(D4);
  DigitalOut led_2(LED2);
  UnbufferedSerial gpsSerial(NC, USBTX, 9600);
  BufferedSerial gpsSerial2(NC,)
  int i = 0;
  char buf[83] = {'\0'};

void gps_signal(){
  char c;
 // while(gpsSerial.readable()){
    gpsSerial.read(&c,1);

    if (i < 83) buf[i++] = c;
    //LOG("GPS", "read byte");
  //}
  led = !led;
 // LOG("GPS", "Rx signaled");
}
int main(void) {
  gpsSerial.format(8,SerialBase::None,1);
  LOG("ADCS", "Start");
  // ADCS adcs;
  // adcs.startThread();
 // Venus838FLPx        gps1(gpsSerial, PinName(PA_8), PinName(PA_11));
  GPSData_t           gpsData;
  //DigitalIn reciveX(D0);
  mbed_error_status_t error = 0;
  led = 0;
  gpsSerial.attach(&gps_signal);

  while (true) {
    LOG("ADCS", "Main");
    led_2 = !led_2;
    if (i == 83){
      buf[82] = '\0';
      
    }else{
      buf[i+1] = '\0';
    }
      LOG("GPS", "%s", buf);
      LOG("GPS", "bytes recived: %d\n", i);
      i = 0;
  //  DEBUG("ADCS", "RX: %d\n", reciveX.read());
    //printf("put someting on serial\n");
    
   // if (gpsSerial.readable()){
   //    led = !led;
   // }
    //error = gps1.read(gpsData);
    // if (!error) {
    //   LOG("GPS",
    //       "Latitude: %f\n   Longitude: %f\n Altitude: %f\n  Ground Speed: %f  "
    //       "\n "
    //       "Current Time: %d",
    //       gpsData.latitude, gpsData.longitude, gpsData.altitude,
    //       gpsData.groundSpeed, gpsData.time);
    //}
    ThisThread::sleep_for(1s);
  }
}
