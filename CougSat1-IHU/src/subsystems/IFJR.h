#ifndef SRC_SUBSYSTEMS_IFJR_H_
#define SRC_SUBSYSTEMS_IFJR_H_

#include "Subsystem.h"

#include <mbed.h>

class IFJR : public Subsystem {
public:
  IFJR(const IFJR &) = delete;
  IFJR & operator=(const IFJR &) = delete;

  IFJR(I2C & i2c);
  ~IFJR();

  mbed_error_status_t initialize();

private:
  I2C & i2c;
};

#endif /* SRC_SUBSYSTEMS_IFJR_H_ */