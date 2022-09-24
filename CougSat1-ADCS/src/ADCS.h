#ifndef ADCS_H
#define ADCS_H

#include "Configuration.h"
#include "interfaces/CDH.h"
#include <mbed.h>
#include <rtos.h>
#include "vectorFunctions.h"
#include <BNO055.h>
#include <photodiode.h>
// #include "kalmanFilterMultiplicative.h"
#include <ESKF.h>
#include <Eigen.h>

using namespace std;

#define PI 3.141592653589793238

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
  uint32_t stackSize = 8192;

  IMUValueSet_t magData;
  IMUValueSet_t gyroData;
  BNO055_EULER_TypeDef eulerData;
  BNO055_QUATERNION_TypeDef quatData;
  voltages* volts;

public:
  ADCS();
  void startThread();
  void attitudeDetermination();
  void attitudeDeterminationLoop();
  void attitudeControl();
};
#endif /* ADCS_H */