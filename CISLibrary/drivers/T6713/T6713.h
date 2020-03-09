#ifndef _LIBRARY_DRIVER_T6713_H_
#define _LIBRARY_DRIVER_T6713_H_

// See
// http://www.co2meters.com/Documentation/Manuals/Manual-AMP-0002-T6713-Sensor.pdf
// for more detailed documentation of the sensor itself

#include <mbed.h>

// T6713 CO2 Sensor I2C default slave address
const uint8_t DEFAULT_T6713_SLAVE_ADDRESS = 0x15 << 1;
// Same command found in the T67XX CO2 Sensor Module documentation, see pg. 29
static const char READ_CO2_COMMAND[5] = {0x04, 0x13, 0x8B, 0x00, 0x01};

class T6713 {
public:
  T6713(const T6713 &) = delete;
  T6713 & operator=(const T6713 &) = delete;

  /**
   * @brief Construct a new T6713 object
   *
   * @param i2c  The I2C with which the driver works
   * @param addr The slave address of the sensor on the i2c, defaults to
   * DEFAULT_T6713_SLAVE_ADDRESS
   */
  T6713(I2C & i2c, uint8_t addr = DEFAULT_T6713_SLAVE_ADDRESS) :
    i2c(i2c), addr(addr) {};

  ~T6713();

  /**
   * @brief Gets the PPM (CO2 level) reading of the sensor
   *
   * @param outValue Reference to the int variable to be changed to the PPM
   * value
   * @return mbed_error_status_t MBED_SUCCESS if the reading was successful,
   * otherwise the error status of the reading
   */
  mbed_error_status_t readPPM(int & outValue);

private:
  /**
   * @brief The I2C with which the driver works to read the sensor values
   *
   */
  I2C & i2c;
  /**
   * @brief The slave address of the sensor to which the I2C should read/write
   *
   */
  uint8_t addr;
};

#endif /* _LIBRARY_DRIVER_T6713_H_ */