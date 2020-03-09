#ifndef _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_
#define _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_

#define DRV8837_PWM_PERIOD_US 100 // PWM period, 10kHz

#include "HBridge.h"
#include <mbed.h>

class DRV8837 : public HBridge {
public:
  DRV8837(const DRV8837 &) = delete;
  DRV8837 & operator=(const DRV8837 &) = delete;

  DRV8837(PinName fwd, PinName rev, PinName nSleep,
      uint8_t period = DRV8837_PWM_PERIOD_US);
  ~DRV8837();

  mbed_error_status_t set(double value, bool /*blocking*/ = true);
  mbed_error_status_t stop(bool brake = true, bool /*blocking*/ = true);
  void                setSleep(bool sleep);

private:
  PwmOut     fwd;
  PwmOut     rev;
  DigitalOut nSleep;
};

#endif /* _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_ */