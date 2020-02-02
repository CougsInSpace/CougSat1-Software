#ifndef _LIBRARY_DRIVER_IMU_IMU_H_
#define _LIBRARY_DRIVER_IMU_IMU_H_

#include <mbed.h>

struct IMUValueSet_t {
  double x          = nan("");
  double y          = nan("");
  double z          = nan("");
  double w          = nan("");
  bool   quaternion = true;
};

struct IMUData_t {
  IMUValueSet_t mag;
  IMUValueSet_t gyro;
  IMUValueSet_t accel;
};

class IMU {
public:
  IMU(const IMU &) = delete;
  IMU & operator=(const IMU &) = delete;

  /**
   * @brief Construct a new IMU object
   *
   */
  IMU() {}

  /**
   * @brief Destroy the IMU object
   *
   */
  virtual ~IMU() {};

  /**
   * @brief Read a IMU's sensors and update fields
   *
   * @param data struct to return result
   * @param blocking will wait until data is present if true
   * @return mbed_error_status_t
   */
  mbed_error_status_t read(IMUData_t & data, bool blocking = true) {
    mbed_error_status_t result = readMag(data.mag, blocking);
    if (result)
      return result;
    result = readGyro(data.gyro, blocking);
    if (result)
      return result;
    result = readAccel(data.accel, blocking);
    if (result)
      return result;
    return MBED_SUCCESS;
  }

  /**
   * @brief Read magnetometer
   *
   * @param data struct to return result
   * @param blocking will wait until data is present if true
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t readMag(
      IMUValueSet_t & data, bool blocking = true) = 0;

  /**
   * @brief Read gyroscope
   *
   * @param data struct to return result
   * @param blocking will wait until data is present if true
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t readGyro(
      IMUValueSet_t & data, bool blocking = true) = 0;

  /**
   * @brief Read accelerometer
   *
   * @param data struct to return result
   * @param blocking will wait until data is present if true
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t readAccel(
      IMUValueSet_t & data, bool blocking = true) = 0;
};

#endif /* _LIBRARY_DRIVER_IMU_IMU_H_ */