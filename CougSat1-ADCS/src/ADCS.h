#ifndef ADCS_H
#define ADCS_H

#include "Configuration.h"
#include "interfaces/CDH.h"
#include <mbed.h>
#include <rtos.h>
#include "vectorFunctions.h"
#include <BNO055.h>
#include <photodiode.h>
#include "kalmanFilterMultiplicative.h"
// #include <ESKF.h>
// #include <Eigen.h>
#include "DRV8837.h"
#include "AD7291.h"
// #include "CISBoard/PinNames.h"// there are multiple pinnames.h files for some reason
#include "PinNames.h"

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
  // CDH    cdh;
  void   cdhThread();
  I2C    sensorBus;
  BNO055 imu;
  uint32_t stackSize = 8192;

  DRV8837 coilX;
  DRV8837 coilY;
  DRV8837 coilZ;
  AD7291 adc;

  IMUValueSet_t magData;
  IMUValueSet_t gyroData;
  BNO055_EULER_TypeDef eulerData;
  BNO055_QUATERNION_TypeDef quatData;
  voltages* volts;
  
  float dt;
  Vector3f lastMag;
  const float maxMTCurrent = .1; // amps

public:
  ADCS(float dtInit);
  void startThread();
  void attitudeDetermination();
  void attitudeDeterminationLoop();
  void attitudeControl();
  void updateSensors();

  short adcsMode; // 0 is off, 1 is bDot, 2 is charge, 3 is bbq roll, 4 is antenna (?)
};
#endif /* ADCS_H */