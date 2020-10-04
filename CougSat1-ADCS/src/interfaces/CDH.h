#ifndef _SRC_INTERFACES_CDH_H_
#define _SRC_INTERFACES_CDH_H_

#include "Configuration.h"
#include <mbed.h>
#include <rtos.h>

class CDH {
public:
  CDH(uint8_t addr, PinName sda, PinName scl);
  void   readCDH();
  void   writeCDH();
  char * getMessage();
  void   setReply(char * input);
  bool   messageReceived();
  bool   messageRequested();

private:
  I2CSlave i2c;
  char     message[MESSAGELENGTH];
  char     reply[MESSAGELENGTH];
};

#endif /* _SRC_INTERFACES_CDH_H_ */