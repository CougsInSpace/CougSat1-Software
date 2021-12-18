#ifndef _SRC_INTERFACES_CDH_H_
#define _SRC_INTERFACES_CDH_H_

#include "Configuration.h"
#include <mbed.h>
#include <rtos.h>


class CDH {
public:
  CDH(uint8_t addr, PinName sda, PinName scl);
  void   readI2C();
  void   writeI2C();
  char * getMessage();
  bool   messageReceived();
  bool   messageRequested();

private:
  I2CSlave i2c;
  char     message[MESSAGELENGTH];
};

#endif /* _SRC_INTERFACES_CDH_H_ */