#ifndef ADCS_H
#define ADCS_H

#include "Configuration.h"
#include "interfaces/CDH.h"
#include <mbed.h>
#include <rtos.h>
#include "vectorFunctions.h"
#include <BNO055.h>
#include <photodiode.h>

using namespace std;

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

  IMUValueSet_t magData;
  BNO055_EULER_TypeDef eulerData;
  BNO055_QUATERNION_TypeDef quatData;
  Photodiodes photodiodes(A3, A3, A2, A2, A1, A1);
  voltages* volts;

public:
  ADCS();
  void startThread();
  void attitudeDetermination();
};
#endif /* ADCS_H */