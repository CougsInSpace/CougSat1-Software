#include "ADCS.h"
// #include "CISBoard/PinNames.h"


/**
 * @brief Construct a new ADCS::ADCS object
 *
 */
// #define TEST_IHU_ADDRESS 0xAC
// #define BUS_I2C0_SDA PinName(25)
// #define BUS_I2C0_SCL PinName(24)
ADCS::ADCS(float dtInit) : sensorBus(ADCS_I2C0_SCL,ADCS_I2C0_SDA), imu(sensorBus,NC,(0X28<<1)), coilX(COIL_X_FWD, COIL_X_REV, COIL_X_SLEEP_N), coilY(COIL_Y_FWD, COIL_Y_REV, COIL_Y_SLEEP_N), coilZ(COIL_Z_FWD, COIL_Z_REV, COIL_Z_SLEEP_N), adc(sensorBus, AD7291Addr_t::HH) {
  this->dt = dtInit;
  // monitor.set_priority(osPriorityNormal);
  // cdhRead.set_priority(osPriorityNormal);
  // cdhRead.start(callback(this, &ADCS::cdhThread));
  // attitudeDeterminationThread.start(callback(this, &ADCS::attitudeDetermination));
  attitudeControlThread.start(callback(this,&ADCS::attitudeControl));
  // monitor.start(callback(this, &ADCS::monitorThread));
}

/**
 * @brief Function for cdh communication thread
 *
 */
// void ADCS::cdhThread() {
//   while (true) {
//     if (cdh.messageReceived()) {
//       cdh.readI2C();
//     }
//     if (cdh.messageRequested()) {
//       cdh.writeI2C();
//     }
//     ThisThread::sleep_for(PERIOD_MS_POLLING_SLEEP);
//   }
// }

void ADCS::attitudeDetermination() {
  
  IMUValueSet_t magData;
  BNO055_EULER_TypeDef eulerData;
  BNO055_QUATERNION_TypeDef quatData;
  //photodiod initiamization

  //A1 = -z
  //A2 = +y
  //A3 = +x
  voltages* volts;
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

void ADCS::attitudeControl() {
  lastMag = Vector3f::Zero();
  Matrix3f iPropGain = .5*Matrix3f::Identity(); // TODO tune gain
  Matrix3f mtResistance;
  mtResistance << 28, 0, 0,
                  0, 28, 0,
                  0, 0, 28; // ohms TODO measure exact value, maybe make into gain matrix
  while(true) {
    // find bDot dipole direction
    IMUValueSet_t magData;
    this->updateSensors();
    
    Vector3f magNorm(magData.x, magData.y, magData.z); //  units of micro teslas
    magNorm.normalize();
    Vector3f bDot = (magNorm - lastMag) / this->dt;
    this->lastMag = magNorm;

    Vector3f iTarget = saturate(bDot,-1*this->maxMTCurrent, this->maxMTCurrent); // set target dipole

    // control current through magnetorquers
    double voltX;
    double voltY;
    double voltZ;
    adc.readVoltage(ADCChannel_t::CM_00, voltX); // read voltage through x mt
    adc.readVoltage(ADCChannel_t::CM_01, voltY); // read voltage through y mt
    adc.readVoltage(ADCChannel_t::CM_02, voltZ); // read voltage through z mt
    Vector3f iMagnetorquer(voltX, voltY, voltZ); 
    iMagnetorquer = mtResistance.inverse() * iMagnetorquer;// TODO find conversion from voltage to current

    Vector3f iError = iTarget - iMagnetorquer; // 

    Vector3f vCommand = saturate(mtResistance*(iTarget + iPropGain*iError), -3.3, 3.3); // commanded mt voltage

    // set coil PWM
    coilX.set(vCommand(0) / 3.3);
    coilY.set(vCommand(1) / 3.3);
    coilZ.set(vCommand(2) / 3.3);
  }
}

void ADCS::updateSensors() {
  imu.readMag(magData);
  imu.readGyro(gyroData);
  imu.get_Euler_Angles(&eulerData);
  imu.get_quaternion(&quatData);
  return;
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