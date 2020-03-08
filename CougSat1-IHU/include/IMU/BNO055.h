#ifndef _LIBRARY_DRIVER_IMU_BNO055_H_
#define _LIBRARY_DRIVER_IMU_BNO055_H_

#include "IMU.h"

#include <mbed.h>

// H = high, F = float, L = low
enum class BNO055Addr_t : uint8_t { H = 0x29 << 1, L = 0x28 << 1 };

class BNO055 : public IMU {
public:
  BNO055(const BNO055 &) = delete;
  BNO055 & operator=(const BNO055 &) = delete;

  BNO055(I2C & i2c, BNO055Addr_t addr);
  ~BNO055();

  mbed_error_status_t readMag(IMUValueSet_t & data, bool blocking = true);
  mbed_error_status_t readGyro(IMUValueSet_t & data, bool blocking = true);
  mbed_error_status_t readAccel(IMUValueSet_t & data, bool blocking = true);

private:
  I2C &        i2c;
  BNO055Addr_t addr;
};

#endif /* _LIBRARY_DRIVER_IMU_IMU_H_ */