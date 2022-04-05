#include "ADCS.h"

using Eigen::MatrixXd; 

/**
 * @brief Construct a new ADCS::ADCS object
 *
 */
#define TEST_IHU_ADDRESS 0xAC
#define BUS_I2C0_SDA PinName(25)
#define BUS_I2C0_SCL PinName(24)
ADCS::ADCS() : cdh(TEST_IHU_ADDRESS, BUS_I2C0_SDA, BUS_I2C0_SCL), sensorBus(D14,D15), imu(sensorBus,NC,(0X28<<1)) {
    // monitor.set_priority(osPriorityNormal);
  // cdhRead.set_priority(osPriorityNormal);
  cdhRead.start(callback(this, &ADCS::cdhThread));
  attitudeDeterminationThread.start(callback(this, &ADCS::attitudeDetermination));
  // monitor.start(callback(this, &ADCS::monitorThread));
}

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

void ADCS::attitudeDetermination() {
  IMUValueSet_t magData;
  BNO055_EULER_TypeDef eulerData;
  BNO055_QUATERNION_TypeDef quatData;
  //photodiod initiamization
  Photodiodes photodiodes(A1, A1, A2, A2, A3, A3);
  voltages* volts;

  while (true) {
    
    // code to read magnetometer

    imu.readMag(magData);
    imu.get_Euler_Angles(&eulerData);
    imu.get_quaternion(&quatData);
    
    // code to read photodiodes
    volts = photodiodes.getVoltages();

    // print input values
    printf("Mag Data X: %lf, Y: %lf, Z: %lf\r\n", magData.x, magData.y, magData.z);
    printf("Euler Angle X: %lf, Y: %lf, Z: %lf\r\n", eulerData.h, eulerData.p, eulerData.r);
    printf("Quat Data X: %lf, Y: %lf, Z: %lf, W: %lf\r\n", quatData.x, quatData.y, quatData.z, quatData.w);

    printf("Photodiodes X: %f, Y: %f, Z: %f\r\n", volts->volt_pos_x, volts->volt_pos_y, volts->volt_pos_z);


    // code to determine orientation

    ThisThread::sleep_for(5s); 
    printf("\n\n");   
  }
}

/**
 * @brief main function for ADCS object
 *
 */
void ADCS::startThread() {
  printf("Outside Main!\r\n");
  DigitalOut led1(PinName(5));
  while (true) {
    //printf("Process Main!\r\n");
    led1 = !led1;
    ThisThread::sleep_for(1000);
    // ThisThread::flags_wait_all(0x00);
  }
}