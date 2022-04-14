#include "ADCS.h"



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
  Photodiodes photodiodes(A3, A3, A2, A2, A1, A1);
  voltages* volts;

  // initial orientation
  imu.readMag(magData);
  ThisThread::sleep_for(1s);
  imu.readMag(magData);
  imu.get_Euler_Angles(&eulerData);
  imu.get_quaternion(&quatData);
  
  // code to read photodiodes
  volts = photodiodes.getVoltages();


  Eigen::Vector3f magi(magData.x, magData.y, magData.z);
  Eigen::Vector3f suni(volts->volt_pos_x, volts->volt_pos_y, volts->volt_pos_z);

  int t = 0;
  while (true) {
    
    // code to read magnetometer

    imu.readMag(magData);
    imu.get_Euler_Angles(&eulerData);
    imu.get_quaternion(&quatData);
    

    // code to read photodiodes
    volts = photodiodes.getVoltages();


    Eigen::Vector3f magf(magData.x, magData.y, magData.z);
    Eigen::Vector3f sunf(volts->volt_pos_x, volts->volt_pos_y, volts->volt_pos_z);
    Eigen::Vector3f magfNorm = magf.normalized();
    Eigen::Vector3f sunfNorm = sunf.normalized();

    // std::cout << "magi: " << magi.normalized() << std::endl;
    // std::cout << "magfNorm: " << magfNorm << std::endl;
    // std::cout << "suni: " << suni.normalized() << std::endl;
    // std::cout << "sunfNOrm: " << sunfNorm << std::endl;

    // Vector3f x1i(-.14, -.21, .97);
    // Vector3f x1f(-.12, -.22, .97);
    // Vector3f x2i(.24,.64,.73);
    // Vector3f x2f(.47,.69,.55);

    // code to determine orientation
    Eigen::Quaternionf qAttitude = determineAttitude(magi.normalized(), magfNorm, suni.normalized(), sunfNorm);

    //Eigen::Quaternionf qAttitude = determineAttitude(x1i, x1f, x2i, x2f);
    Eigen::Vector3f eulerAttitude = qAttitude.toRotationMatrix().eulerAngles(0,1,2);


    // print input values
    // printf("Mag Data X: %lf, Y: %lf, Z: %lf\r\n", magData.x, magData.y, magData.z);
   // printf("Mag Data X Norm: %lf, Y: %lf, Z: %lf\r\n", magfNorm[0], magfNorm[1], magfNorm[2]);
    // printf("Euler Angle X: %lf, Y: %lf, Z: %lf\r\n", eulerData.h, eulerData.p, eulerData.r);
    // printf("Quat Data X: %lf, Y: %lf, Z: %lf, W: %lf\r\n", quatData.x, quatData.y, quatData.z, quatData.w);
    // printf("Photodiodes X: %f, Y: %f, Z: %f\r\n", volts->volt_pos_x, volts->volt_pos_y, volts->volt_pos_z);
    //printf("Photodiodes X Norm: %f, Y: %f, Z: %f\r\n", sunfNorm[0], sunfNorm[1], sunfNorm[2]);
    printf("Calculated Attitude Euler X: %f, Y: %f, Z: %f\r\n", eulerAttitude[0], eulerAttitude[1], eulerAttitude[2]);
    printf("Calculated Attitude Quaternion W: %f, X: %f, Y: %f, Z: %f\r\n", qAttitude.w(), qAttitude.x(), qAttitude.y(), qAttitude.z());
    printf("\n\n"); 
    
    
    ThisThread::sleep_for(5s);   
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