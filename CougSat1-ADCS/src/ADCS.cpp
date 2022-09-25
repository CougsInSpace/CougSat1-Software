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
  // cdhRead.start(callback(this, &ADCS::cdhThread));
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
  Photodiodes photodiodes(A3, A3, A2, A2, A1, A1);

  photodiodes.setOffset(.002, .002, .002, .002, .008, .008);

  // initial orientation
  imu.readMag(magData);
  imu.readGyro(gyroData);
  printf("Waiting for IMU warmup\n");
  ThisThread::sleep_for(10s);
  imu.readMag(magData);
  imu.readGyro(gyroData);
  imu.get_Euler_Angles(&eulerData);
  imu.get_quaternion(&quatData);
  
  // code to read photodiodes
  volts = photodiodes.getVoltages();

  // Kalman filter 1 stuff
  // SatelliteState currentState(0,0,0,1,0,0,0);
  // MatrixXf stateCovariance{{0, 0, 0, 0, 0, 0},
  //                          {0, 0, 0, 0, 0, 0},
  //                          {0, 0, 0, 0, 0, 0},
  //                          {0, 0, 0, 0, 0, 0},
  //                          {0, 0, 0, 0, 0, 0},
  //                          {0, 0, 0, 0, 0, 0}};
  // MatrixXf procNoiseCovariance{{.1,.1,.1,.1,.1,.1},
  //                              {.1,.1,.1,.1,.1,.1},
  //                              {.1,.1,.1,.1,.1,.1},
  //                              {.1,.1,.1,.1,.1,.1},
  //                              {.1,.1,.1,.1,.1,.1},
  //                              {.1,.1,.1,.1,.1,.1}};
  // Matrix3f magneticCovMat{{ 0.29629224,  0.2503048,  -0.38154479},
  //                         { 0.2503048,   0.21185839, -0.32251307},
  //                         {-0.38154479, -0.32251307,  0.49184944}};
  // Matrix3f sunCovMat{{ 0.36135865,  0.45814491, -0.12726127},
  //                    { 0.45814491,  0.58427518, -0.16126741},
  //                    {-0.12726127, -0.16126741,  0.05436621}};

  //Kalman filter 2 stuff
  Vector3d magi(magData.x, magData.y, magData.z);
  Vector3d suni(volts->volt_pos_x, volts->volt_pos_y, -1 * volts->volt_pos_z);
  Vector3d magiNorm = magi.normalized();
  Vector3d suniNorm = suni.normalized();

  Vector3d wk_B2I_B(gyroData.x, gyroData.y, gyroData.z);
  Quaterniond qk_I2B(1,0,0,0);
  Vector3d biask(0,0,0);
  bool eclp = true;
  float dt = .25;
  float sig_r = 3; // standard deviation of noise
  float sig_w = 1; // standard deviation of bias
  Matrix3d R_ss = .5*Matrix3d::Identity();
  Matrix3d R_m = .5*Matrix3d::Identity(); // IF THE FACTOR IS EXTREMELY HIGH IT WILL STAY STABLE FOR LONGER, BUT ANY MOTION STILL HAS ERRONEOUS RESULTS
  MatrixXd Pk = 1*MatrixXd::Identity(6,6);

  // Eigen::Matrix<float, 3, 3> W;
  // W << 0,0,0,0,0,0,0,0,0;
  // Eigen::Matrix<float, 3, 1> V;
  // V << 0,0,0;
  // float incl = 0; //  no idea what this is
  // float B = 1; // geomagnetic field strength

  // IMU_EKF::ESKF<float> kalmanFilter;
  // kalmanFilter.initWithAcc(magiNorm(0), magiNorm(1), magiNorm(2));
  while (true) { 
    // using namespace std::chrono_literals;

    // std::future<int> threadObj = std::async(test_func); // Start thread running
    // std::this_thread::sleep_for(std::chrono::milliseconds(100)); //may need to be adjusted

    // std::future_status status = threadObj.wait_for(0ms); // check if thread is done
    // if (status == std::future_status::ready) { 
    //     int result = threadObj.get(); // join thread
    // } else {
    //     std::cout<<"TODO: attitudeDeterminationLoop took too long, error"<<std::endl;
    //     break;
    // }

    // code to read BNO055 sensor
    imu.readMag(magData);
    imu.readGyro(gyroData);
    imu.get_Euler_Angles(&eulerData);
    imu.get_quaternion(&quatData);

    // code to read photodiodes
    volts = photodiodes.getVoltages();

    // Create vectors from measurements
    Vector3d magf(magData.x, magData.y, magData.z); //  units of micro teslas
    Vector3d sunf(volts->volt_pos_x, volts->volt_pos_y, -1* volts->volt_pos_z);
    // Vector3f omega(gyroData.x, gyroData.y, gyroData.z); // units of degrees per second, need to stay that way apparently
    Vector3d omega = Vector3d::Zero();
    // omega *= (PI / 180.0);
    Vector3d magfNorm = magf.normalized();
    Vector3d sunfNorm = sunf.normalized();

    printf("x\r\n");
    // kalmanFilter.predict(dt);
    // cout << "test" << endl;
    // kalmanFilter.correctGyr(omega(0), omega(1), omega(2));
    // cout << "test" << endl;
    // if (eclp == false) {
    //   kalmanFilter.correctMeasurement(suniNorm(0), suniNorm(1), suniNorm(2), sunfNorm(0), sunfNorm(1), sunfNorm(2));
    // }
    // cout << "test" << endl;
    // kalmanFilter.correctMeasurement(magiNorm(0), magiNorm(1), magiNorm(2),magfNorm(0), magfNorm(1), magfNorm(2));
    // cout << "test" << endl;
    // kalmanFilter.reset();
    // cout << "test" << endl;


    // Quaternionf q = kalmanFilter.getAttitude(); // order xyzw

    // Code to determine orientation (only for mag sensor right now)
    ReturnKalman returnVars =  multiplicativeFilter(
      omega, 
      qk_I2B,
      biask, 
      sunfNorm, 
      suniNorm,
      eclp,
      magfNorm,
      magiNorm,
      dt,
      sig_r,
      sig_w,
      R_ss,
      R_m,
      Pk
    );

    wk_B2I_B = returnVars.wk1_B2I_B;
    qk_I2B = returnVars.qk1_I2B;
    biask = returnVars.biask1;
    Pk = returnVars.Pk1;
     
    // Quaterniond qAttitude = determineAttitude(magi.normalized(), magfNorm, suni.normalized(), sunfNorm);


    // Print statements for pyserial
    // cout << "omega" << endl;
    cout << omega << endl;
    // cout << "wk_B2I_B" << endl;
    // cout << wk_B2I_B << endl;
    // cout << "biask" << endl;
    // cout << biask << endl;
    cout << magfNorm << endl;
    // cout << magf<< endl;
    // cout << "mag above" << endl;
    cout << qk_I2B << endl;

    // cout << magfNorm << endl;

    ThisThread::sleep_for(250ms);

    // time_t endTime = time();
    // printf("Took %d seconds to run loop", endTime - startTime);
  }
}

void ADCS::attitudeDeterminationLoop() {
//   time_t startTime = time();

//   // code to read magnetometer
  
//   imu.readMag(magData);
//   imu.get_Euler_Angles(&eulerData);
//   imu.get_quaternion(&quatData);
  

//   // code to read photodiodes
//   volts = photodiodes.getVoltages();


//   Eigen::Vector3f magf(magData.x, magData.y, magData.z);
//   Eigen::Vector3f sunf(volts->volt_pos_x, volts->volt_pos_y, -1* volts->volt_pos_z);
//   Eigen::Vector3f magfNorm = magf.normalized();
//   Eigen::Vector3f sunfNorm = sunf.normalized();

//   // std::cout << "magi: " << magi.normalized() << std::endl;
//   // std::cout << "magfNorm: " << magfNorm << std::endl;
//   // std::cout << "suni: " << suni.normalized() << std::endl;
//   // std::cout << "sunfNOrm: " << sunfNorm << std::endl;

//   // Vector3f x1i(-.14, -.21, .97);
//   // Vector3f x1f(-.12, -.22, .97);
//   // Vector3f x2i(.24,.64,.73);
//   // Vector3f x2f(.47,.69,.55);

//   // code to determine orientation
//   Eigen::Quaternionf qAttitude = determineAttitude(magi.normalized(), magfNorm, suni.normalized(), sunfNorm);

//   //Eigen::Quaternionf qAttitude = determineAttitude(x1i, x1f, x2i, x2f);
//   Eigen::Vector3f eulerAttitude = qAttitude.toRotationMatrix().eulerAngles(0,1,2);



//   // print input values
//   // printf("Mag Data X: %lf, Y: %lf, Z: %lf\r\n", magData.x, magData.y, magData.z);
//   // printf("Mag Data X Norm: %lf, Y: %lf, Z: %lf\r\n", magfNorm[0], magfNorm[1], magfNorm[2]);
//   // printf("Euler Angle Heading: %lf, Pitch: %lf, Roll: %lf\r\n", eulerData.h, eulerData.p, eulerData.r); //from imu
//   //printf("Calculated Attitude Euler X: %lf, Y: %lf, Z: %lf\r\n", eulerAttitude[0], eulerAttitude[1], eulerAttitude[2]); // from math

//   //printf("Quat Data X: %lf, Y: %lf, Z: %lf, W: %lf\r\n", quatData.x, quatData.y, quatData.z, quatData.w);// from imu
//   // printf("Photodiodes X: %f, Y: %f, Z: %f\r\n", volts->volt_pos_x, volts->volt_pos_y, -1 * volts->volt_pos_z);
//   // printf("Photodiodes Norm X: %f, Y: %f, Z: %f\r\n", sunfNorm[0], sunfNorm[1], sunfNorm[2]);
//   // printf("Calculated Attitude Quaternion W: %lf, X: %lf, Y: %lf, Z: %lf\r\n", qAttitude.w(), qAttitude.x(), qAttitude.y(), qAttitude.z()); //from math
//   // printf("\n\n"); 


//   // Print statements for pyserial
//   printf("x\r\n");
//   printf("%f\r\n%f\r\n%f\r\n", sunfNorm[0], sunfNorm[1], sunfNorm[2]);
//   printf("%f\r\n%f\r\n%f\r\n", magfNorm[0], magfNorm[1], magfNorm[2]);
//   // printf("%lf\n\r", volts->volt_pos_x);
//   printf("%f\r\n%f\r\n%f\r\n%f\r\n", qAttitude.w(), qAttitude.x(), qAttitude.y(), qAttitude.z());
  

//   //ThisThread::sleep_for(100ms);  

//   time_t endTime = time();
//   printf("Took %d seconds to run loop", endTime - startTime);
}

void ADCS::attitudeControl() {

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