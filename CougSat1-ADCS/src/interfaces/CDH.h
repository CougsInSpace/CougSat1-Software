#ifndef _SRC_INTERFACES_CDH_H_
#define _SRC_INTERFACES_CDH_H_

#include <mbed.h>
#include <rtos.h>
#include "Configuration.h"

class CDH {
public:
  CDH(uint8_t addr, PinName sda, PinName scl);
  void readCDH();
  void writeCDH();
  char* getMessage();
  bool messageReceived();
  bool messageRequested();

private:
  I2CSlave i2c;
  char message[MESSAGELENGTH];
};

#endif /* _SRC_INTERFACES_CDH_H_ */