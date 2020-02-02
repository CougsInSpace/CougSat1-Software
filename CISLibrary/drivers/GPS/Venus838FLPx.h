#ifndef _LIBRARY_DRIVER_GPS_VENUS838FLPX_H_
#define _LIBRARY_DRIVER_GPS_VENUS838FLPX_H_

#include "GPS.h"

#include <mbed.h>
#include <string.h>
#include <stdlib.h>

class Venus838FLPx : GPS {
public:
  Venus838FLPx(const Venus838FLPx &) = delete;
  Venus838FLPx & operator=(const Venus838FLPx &) = delete;

  Venus838FLPx(Serial & serial, PinName reset, PinName pulse);
  ~Venus838FLPx();

  mbed_error_status_t read(GPSData_t & data, bool blocking = true);

private:

  mbed_error_status_t parseGGA(GPSData_t & data, char *nmeaString);
  mbed_error_status_t parseGLL(GPSData_t & data, char *nmeaString);
  mbed_error_status_t parseGSA(GPSData_t & data, char *nmeaString);
  mbed_error_status_t parseRMC(GPSData_t & data, char *nmeaString);
  mbed_error_status_t parseVTG(GPSData_t & data, char *nmeaString);
  mbed_error_status_t parseZDA(GPSData_t & data, char *nmeaString);

  Serial &   serial;
  DigitalOut reset;
  DigitalIn  pulse;
};

#endif /* _LIBRARY_DRIVER_GPS_VENUS838FLPX_H_ */