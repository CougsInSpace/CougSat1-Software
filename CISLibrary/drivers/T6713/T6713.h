#ifndef _LIBRARY_DRIVER_T6713_H_
#define _LIBRARY_DRIVER_T6713_H_

#include <mbed.h>

// T6173 CO2 Sensor i2C Address
const uint8_t DEFAULT_T6713_SLAVE_ADDRESS 0x15 << 1;
// Same command found in the T67XX CO2 Sensor Module documentation, see pg. 29
static const uint8_t READ_CO2_COMMAND = {0x04, 0x13, 0x8B, 0x00, 0x01};

class T6713 {
public:
  T6713(const T6173 &) = delete;
  T6173 & operator=(const T6173 &) = delete;

  /**
   * @brief Construct a new T6713 object
   *
   * @param i2c  The I2C with which the driver works
   * @param addr The slave address of the sensor on the i2c
   */
  T6713(I2C & i2c, uint8_t addr = DEFAULT_T6713_SLAVE_ADDRESS);

  ~T6173();

  /**
   * @brief Reads the current PPM (CO2 level) that the sensor reads
   *
   * @return int The PPM reading of the sensor
   */
  int readPPM();

private:
  /**
   * @brief The I2C with which the driver works to read the sensor values
   *
   */
  I2C i2c;
  /**
   * @brief The slave address of the sensor on which the I2C should read/write
   *
   */
  uint8_t addr;
};

#endif /* _LIBRARY_DRIVER_T6713_H_ */