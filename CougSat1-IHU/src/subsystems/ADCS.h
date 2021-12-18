#ifndef SRC_SUBSYSTEMS_ADCS_H_
#define SRC_SUBSYSTEMS_ADCS_H_

#include "Subsystem.h"

#include <mbed.h>

class ADCS : public Subsystem {
public:
  ADCS(const ADCS &) = delete;
  ADCS & operator=(const ADCS &) = delete;

  ADCS(I2C & i2c);
  ~ADCS();

  mbed_error_status_t initialize();

private:
  I2C & i2c;
};

#endif /* SRC_SUBSYSTEMS_ADCS_H_ */