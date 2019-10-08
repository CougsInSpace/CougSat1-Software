#ifndef _SRC_INTERFACES_CDH_H_
#define _SRC_INTERFACES_CDH_H_

#include <mbed.h>
#include <rtos.h>
#define MESSAGELENGTH (8) //8 byte messages will handle all possible communication

class CDH {
public:
  CDH(uint8_t addr, PinName sda, PinName scl);
  void readCDH();
  char* getMessage();
  bool hasMessage();

private:
  I2CSlave i2c;
  char message[MESSAGELENGTH];
};

#endif /* _SRC_INTERFACES_CDH_H_ */