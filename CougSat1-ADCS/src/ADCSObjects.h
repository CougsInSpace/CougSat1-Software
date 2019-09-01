#ifndef _SRC_PMIC_OBJECTS_H_
#define _SRC_PMIC_OBJECTS_H_

#include "Configuration.h"
#include "interfaces/CDH.h"

#include <mbed.h>

/*************************** Interfaces and Drivers ***************************/
extern I2C      i2cLocal;
extern I2CSlave i2cBus;

extern CDH cdh;

extern DigitalOut statusLED;

extern InterruptIn interruptCtrlSync;
extern InterruptIn interruptBusI2CIRQ;

#endif /* _SRC_PMIC_OBJECTS_H_ */