#ifndef _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_
#define _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_

#include "CISError.h"
#include "HBridge.h"

#include <mbed.h>

class DRV8837 : public HBridge {
public:
  DRV8837(const DRV8837 &) = delete;
  DRV8837 & operator=(const DRV8837 &) = delete;

  DRV8837(PinName fwd, PinName rev, PinName sleep);
  ~DRV8837();

  CISResult_t set(double value, bool blocking = true);
  CISResult_t stop(bool brake = true, bool blocking = true);

private:
  PwmOut     fwd;
  PwmOut     rev;
  DigitalOut sleep;
};

#endif /* _LIBRARY_DRIVER_HBRIDGE_DRV8837_H_ */