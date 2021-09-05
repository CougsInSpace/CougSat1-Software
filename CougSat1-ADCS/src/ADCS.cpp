#include "ADCS.h"
#include "../../CISLibrary/drivers/HBridge/DRV8837.h"
#include "../../CISLibrary/drivers/IMU/BNO055.h"

/**
 * @brief Function for cdh communication thread
 *
 */
void ADCS::cdhThread() {
  while (true) {
    if (cdh.messageReceived()) {
      cdh.readI2C();
    }
    if (cdh.messageRequested()) {
      cdh.writeI2C();
    }
    ThisThread::sleep_for(PERIOD_MS_POLLING_SLEEP);
  }
}

/**
 * @brief Construct a new ADCS::ADCS object
 *
 */
#define TEST_IHU_ADDRESS 0xAC
#define BUS_I2C0_SDA PinName(25)
#define BUS_I2C0_SCL PinName(24)
ADCS::ADCS() : cdh(TEST_IHU_ADDRESS, BUS_I2C0_SDA, BUS_I2C0_SCL) {
  // monitor.set_priority(osPriorityNormal);
  // cdhRead.set_priority(osPriorityNormal);
  cdhRead.start(callback(this, &ADCS::cdhThread));
  // monitor.start(callback(this, &ADCS::monitorThread));
}

/**
 * @brief main function for ADCS object
 *
 */

void ADCS::startThread() {
  // DigitalOut           led1(PinName(5));
  I2C                  imuI2C = I2C(PinName(PB_9), PinName(PB_8));
  BNO055               imu1(imuI2C, PinName(NULL), (0x28 << 1));
  imuData_t            imuData;
  BNO055_EULER_TypeDef angle;
  PwmOut               fwd = PwmOut(PinName(PB_10));
  PwmOut               rev = PwmOut(PinName(PB_4));
  DigitalOut           pSleep{PinName(PA_8)};
  DRV8837              mag1{PinName(PB_10), PinName(PB_4), PinName(PA_8)};
  DEBUG("H-Bridge", "Initialized");
  int target = 30;
  while (true) {
    

        
    mag1.setSleep(false);
    mag1.set(-1);
    /*
    if(angle.h < target){
      mag1.set(1);
      DEBUG("IMU", "SLige to the right %d", angle.h);

    }else if( angle.h > target){
      mag1.set(-1);
      DEBUG("IMU", "SLige to the left %d", angle.h);
    }else{
   //   mag1.setSleep(true);
    }
    if(angle.h != target){
   //   mag1.setSleep(false);
    }
*/
    
    // printf("Process Main!\r\n");
    // led1 = !led1;
    imu1.get_Euler_Angles(&angle);
    imu1.read(imuData);
    LOG("IMU\t", "main\n");
    /*LOG("IMU\n",
        "Magnotometer: x:%f, y:%f, z:%f\n"
        "GyroScope: x:%f, y:%f, z:%f\n"
        "Accelerometer: x:%f, y:%f, z:%f\n"
        "Eular Angles: r:%f, p:%f, h:%f",
        imuData.mag.x, imuData.mag.y, imuData.mag.z, imuData.gyro.x,
        imuData.gyro.y, imuData.gyro.z, imuData.accel.x, imuData.accel.y,
        imuData.accel.z, angle.r, angle.p, angle.h);
        */

    ThisThread::sleep_for(500);
    // ThisThread::flags_wait_all(0x00);
  }
}