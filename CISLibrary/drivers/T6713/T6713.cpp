#include "T6713.h"

/**
 * @brief Construct a new T6713 object
 *
 * @param i2c  The I2C with which the driver works
 */
T6713::T6713(I2C & i2c) : i2c(i2c) {}

/**
 * @brief Destroy the T6713 object.
 * As of now, the destructor does not need anything.
 *
 */
T6713::~T6713() {}

/**
 * @brief Gets the PPM (CO2 level) reading of the sensor
 *
 * @param outValue Reference to the 16-bit int variable to be changed to the
 * CO2 concentration in PPM. This 16-bit int datatype is explicity specified
 * in the sensor's documentation.
 * @return mbed_error_status_t MBED_SUCCESS if the reading was successful,
 * otherwise the error status of the reading
 */
mbed_error_status_t T6713::readPPM(uint16_t & ppmOut) {
  char data[4];

  if (this->i2c.write(T6713::T6713_SLAVE_ADDRESS, T6713::READ_CO2_COMMAND, 5)) {
    return MBED_ERROR_WRITE_FAILED;
  }

  if (this->i2c.read(T6713::T6713_SLAVE_ADDRESS, data, 4)) {
    return MBED_ERROR_READ_FAILED;
  }

  ppmOut = (uint16_t)(((data[2] & 0x3F) << 8) | data[3]);
  return MBED_SUCCESS;
}