#ifndef _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_
#define _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_

#define DRV8837_PWM_FREQ_HZ      10e3  // PWM frequency (Hz), ~10kHz
#define DRV8837_SET_WAIT_TIME_US  1e2  // Time to wait after pwm is set, 0.10 usec for now.

#include "HBridge.h"
#include <mbed.h>

class DRV8837 : public HBridge {
public:
  DRV8837(const DRV8837 &) = delete;
  DRV8837 & operator=(const DRV8837 &) = delete;

  DRV8837(PinName fwd, PinName rev, PinName nSleep);
  ~DRV8837();

  mbed_error_status_t set(double value);
  mbed_error_status_t stop(bool brake = true);

private:
  PwmOut          fwd;
  PwmOut          rev;
  DigitalOut      nSleep;

  void wake();
  void sleep_immediate();
};

#endif /* _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_ */