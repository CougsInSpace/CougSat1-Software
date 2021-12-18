#include "BNO055.h"

/**
 * @brief Construct a new BNO055::BNO055 object
 *
 * @param i2c attached to the IMU
 * @param addr of the IMU
 */
BNO055::BNO055(I2C & i2c, BNO055Addr_t addr) : i2c(i2c), addr(addr) {}

/**
 * @brief Destroy the BNO055::BNO055 object
 *
 */
BNO055::~BNO055() {}

/**
 * @brief Read magnetometer
 *
 * @param data struct to return result
 * @param blocking will wait until data is present if true
 * @return mbed_error_status_t
 */
mbed_error_status_t BNO055::readMag(IMUValueSet_t & data, bool blocking) {
  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Read gyroscope
 *
 * @param data struct to return result
 * @param blocking will wait until data is present if true
 * @return mbed_error_status_t
 */
mbed_error_status_t BNO055::readGyro(IMUValueSet_t & data, bool blocking) {
  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Read accelerometer
 *
 * @param data struct to return result
 * @param blocking will wait until data is present if true
 * @return mbed_error_status_t
 */
mbed_error_status_t BNO055::readAccel(IMUValueSet_t & data, bool blocking) {
  return MBED_ERROR_UNSUPPORTED;
}