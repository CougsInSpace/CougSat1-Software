#ifndef _LIBRARY_DRIVER_T6713_H_
#define _LIBRARY_DRIVER_T6713_H_

#include <mbed.h>

/**
 * @brief The driver class for working with the T6713 sensor. It is specifically
 * responsible for reading the CO2 concentration (measured in PPM) from the
 * sensor.
 * See
 * http://www.co2meters.com/Documentation/Manuals/Manual-AMP-0002-T6713-Sensor.pdf
 * for more detailed documentation of the sensor itself
 *
 * Example:
 * @code
 *   I2C i2c(I2C_SDA, I2C_SCL);
 *   T6713 t6713(i2c);
 *
 *   uint16_t ppmValue = 0;
 *   if (t6713.readPPM(ppmValue) == MBED_SUCCESS) {
 *     printf("PPM value: %d", ppmValue);
 *   }
 * @encode
 *
 */
class T6713 {
public:
  T6713(const T6713 &) = delete;
  T6713 & operator=(const T6713 &) = delete;

  /**
   * @brief Construct a new T6713 object
   *
   * @param i2c  The I2C with which the driver works
   */
  T6713(I2C & i2c);

  /**
   * @brief Destroy the T6713 object.
   * As of now, the destructor does not need anything.
   *
   */
  ~T6713();

  /**
   * @brief Gets the PPM (CO2 level) reading of the sensor
   *
   * @param outValue Reference to the 16-bit int variable to be changed to the
   * CO2 concentration in PPM. This 16-bit int datatype is explicity specified
   * in the sensor's documentation.
   * @return mbed_error_status_t MBED_SUCCESS if the reading was successful,
   * otherwise the error status of the reading
   */
  mbed_error_status_t readPPM(uint16_t & outValue);

private:
  static const uint8_t  T6713_SLAVE_ADDRESS = 0x15 << 1;
  static constexpr char READ_CO2_COMMAND[5] = {0x04, 0x13, 0x8B, 0x00, 0x01};

  I2C & i2c;
};

#endif /* _LIBRARY_DRIVER_T6713_H_ */