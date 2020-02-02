#ifndef _SRC_INTERFACES_CDH_H_
#define _SRC_INTERFACES_CDH_H_

#include <mbed.h>

class CDH {
public:
  CDH(I2CSlave & i2c);

  bool              hasMessage();
  mbed_error_code_t processMessage();

private:
  I2CSlave & i2c;
};

#endif /* _SRC_INTERFACES_CDH_H_ */