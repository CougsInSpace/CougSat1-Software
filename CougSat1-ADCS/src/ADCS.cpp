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
  /*
  IMUValueSet_t magData;
  BNO055_EULER_TypeDef eulerData;
  BNO055_QUATERNION_TypeDef quatData;
  //photodiod initiamization

  //A1 = -z
  //A2 = +y
  //A3 = +x
  Photodiodes photodiodes(A3, A3, A2, A2, A1, A1);
  voltages* volts;
  */

  photodiodes.setOffset(.002, .002, .002, .002, .008, .008);

  // initial orientation
  imu.readMag(magData);
  printf("Waiting for IMU warmup\n");
  ThisThread::sleep_for(10s);
  imu.readMag(magData);
  imu.get_Euler_Angles(&eulerData);
  imu.get_quaternion(&quatData);
  
  // code to read photodiodes
  volts = photodiodes.getVoltages();


  Eigen::Vector3f magi(magData.x, magData.y, magData.z);
  Eigen::Vector3f suni(volts->volt_pos_x, volts->volt_pos_y, -1 * volts->volt_pos_z);
  
  while (true) { 
    using namespace std::chrono_literals;

    std::future<int> threadObj = std::async(test_func);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //may need to be adjusted

    std::future_status status = threadObj.wait_for(0ms);
    if (status == std::future_status::ready) {
        int result = threadObj.get();
    } else {
        std::cout<<"TODO: attitudeDeterminationLoop took too long, error"<<std::endl;
        break;
    }
  }
}

void ADCS::attitudeDeterminationLoop() {
  time_t startTime = time();

  // code to read magnetometer
  
  imu.readMag(magData);
  imu.get_Euler_Angles(&eulerData);
  imu.get_quaternion(&quatData);
  

  // code to read photodiodes
  volts = photodiodes.getVoltages();


  Eigen::Vector3f magf(magData.x, magData.y, magData.z);
  Eigen::Vector3f sunf(volts->volt_pos_x, volts->volt_pos_y, -1* volts->volt_pos_z);
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
  // printf("Euler Angle Heading: %lf, Pitch: %lf, Roll: %lf\r\n", eulerData.h, eulerData.p, eulerData.r); //from imu
  //printf("Calculated Attitude Euler X: %lf, Y: %lf, Z: %lf\r\n", eulerAttitude[0], eulerAttitude[1], eulerAttitude[2]); // from math

  //printf("Quat Data X: %lf, Y: %lf, Z: %lf, W: %lf\r\n", quatData.x, quatData.y, quatData.z, quatData.w);// from imu
  // printf("Photodiodes X: %f, Y: %f, Z: %f\r\n", volts->volt_pos_x, volts->volt_pos_y, -1 * volts->volt_pos_z);
  // printf("Photodiodes Norm X: %f, Y: %f, Z: %f\r\n", sunfNorm[0], sunfNorm[1], sunfNorm[2]);
  // printf("Calculated Attitude Quaternion W: %lf, X: %lf, Y: %lf, Z: %lf\r\n", qAttitude.w(), qAttitude.x(), qAttitude.y(), qAttitude.z()); //from math
  // printf("\n\n"); 


  // Print statements for pyserial
  printf("x\r\n");
  printf("%f\r\n%f\r\n%f\r\n", sunfNorm[0], sunfNorm[1], sunfNorm[2]);
  printf("%f\r\n%f\r\n%f\r\n", magfNorm[0], magfNorm[1], magfNorm[2]);
  // printf("%lf\n\r", volts->volt_pos_x);
  printf("%f\r\n%f\r\n%f\r\n%f\r\n", qAttitude.w(), qAttitude.x(), qAttitude.y(), qAttitude.z());
  

  //ThisThread::sleep_for(100ms);  

  time_t endTime = time();
  printf("Took %d seconds to run loop", endTime - startTime);
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
    ThisThread::sleep_for(1s);
    // ThisThread::flags_wait_all(0x00);
  }
}