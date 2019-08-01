#ifndef _LIBRARY_DRIVER_GPS_VENUS838FLPX_H_
#define _LIBRARY_DRIVER_GPS_VENUS838FLPX_H_

#include "GPS.h"

#include <mbed.h>

class Venus838FLPx : GPS {
public:
  Venus838FLPx(const Venus838FLPx &) = delete;
  Venus838FLPx & operator=(const Venus838FLPx &) = delete;

  Venus838FLPx(Serial & serial, PinName reset, PinName pulse);
  ~Venus838FLPx();

  mbed_error_code_t read(GPSData_t & data, bool blocking = true);

private:
  Serial &   serial;
  DigitalOut reset;
  DigitalIn  pulse;
};

#endif /* _LIBRARY_DRIVER_GPS_VENUS838FLPX_H_ */