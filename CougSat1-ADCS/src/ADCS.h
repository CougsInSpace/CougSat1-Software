#ifndef ADCS_H
#define ADCS_H

#include "Configuration.h"
#include "interfaces/CDH.h"
#include <mbed.h>
#include <rtos.h>
// #include "vectorFunctions.h"
#include <BNO055.h>
#include <photodiode.h>
#include "kalmanFilterMultiplicative.h"
<<<<<<< HEAD
// #include <ESKF.h>
// #include <Eigen.h>
=======
#include <DRV8837.h>
#include "PinNames.h"
>>>>>>> 46c2e1890daed59852beaf924457f839b18a4cc2

using namespace std;
// using namespace IMU_EKF;

#define PI 3.141592653589793238

class ADCS {
private:
  Thread monitor;
  Thread cdhRead;
  Thread attitudeDeterminationThread;
  Thread attitudeControlThread;
  char   message[9];
  CDH    cdh;
  void   cdhThread();
  I2C    sensorBus;
  BNO055 imu;
  uint32_t stackSize = 8192;
  float dt;
  DRV8837 hBridgeCoilX;


  IMUValueSet_t magData;
  IMUValueSet_t gyroData;
  BNO055_EULER_TypeDef eulerData;
  BNO055_QUATERNION_TypeDef quatData;
  voltages* volts;

public:
  ADCS(float dtInit);
  void startThread();
  void attitudeDetermination();
  void attitudeDeterminationLoop();
  void attitudeControl();
};
#endif /* ADCS_H */