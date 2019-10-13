#ifndef _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_
#define _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_

#define DRV8837_PWM_PERIOD_US 100 // PWM period, 10kHz

#include "HBridge.h"
#include <mbed.h>

class DRV8837 : public HBridge {
public:
  DRV8837(const DRV8837 &) = delete;
  DRV8837 & operator=(const DRV8837 &) = delete;

  DRV8837(PinName fwd, PinName rev, PinName nSleep);
  ~DRV8837();

  mbed_error_status_t set(double value, bool blocking);
  mbed_error_status_t stop(bool brake = true, bool blocking = true);

private:
  PwmOut     fwd;
  PwmOut     rev;
  DigitalOut nSleep;

  void setSleep(bool sleep);
};

#endif /* _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_ */