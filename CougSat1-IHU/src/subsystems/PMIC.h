#ifndef SRC_SUBSYSTEMS_PMIC_H_
#define SRC_SUBSYSTEMS_PMIC_H_

#include "Subsystem.h"

#include <mbed.h>

class PMIC : public Subsystem {
public:
  PMIC(const PMIC &) = delete;
  PMIC & operator=(const PMIC &) = delete;

  PMIC(I2C & i2c);
  ~PMIC();

  mbed_error_status_t initialize();

private:
  I2C & i2c;
};

#endif /* SRC_SUBSYSTEMS_PMIC_H_ */