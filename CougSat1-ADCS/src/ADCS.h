#ifndef ADCS_H
#define ADCS_H

#include "Configuration.h"
#include "interfaces/CDH.h"
#include <mbed.h>
#include <rtos.h>
#include "vectorFunctions.h"
#include <BNO055.h>
#include <photodiode.h>



class ADCS {
private:
  Thread monitor;
  Thread cdhRead;
  Thread attitudeDeterminationThread;
  char   message[9];
  CDH    cdh;
  void   cdhThread();
  I2C    sensorBus;
  BNO055 imu;

public:
  ADCS();
  void startThread();
  void attitudeDetermination();
};
#endif /* ADCS_H */