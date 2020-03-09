#include "T6713.h"

/**
 * @brief Destroy the T6713 object.
 * As of now, the destructor does not need anything.
 *
 */
T6713::~T6713() {}

/**
 * @brief Gets the PPM (CO2 level) reading of the sensor
 *
 * @param outValue Reference to the int variable to be changed to the PPM value
 * @return mbed_error_status_t MBED_SUCCESS if the reading was successful,
 * otherwise the error status of the reading
 */
mbed_error_status_t T6713::readPPM(int & ppmOut) {
  char data[4];

  if (this->i2c.write(this->addr, READ_CO2_COMMAND, 5)) {
    return MBED_ERROR_WRITE_FAILED;
  }

  if (this->i2c.read(this->addr, data, 4)) {
    return MBED_ERROR_READ_FAILED;
  }

  ppmOut = (((data[2] & 0x3F) << 8) | data[3]);
  return MBED_SUCCESS;
}