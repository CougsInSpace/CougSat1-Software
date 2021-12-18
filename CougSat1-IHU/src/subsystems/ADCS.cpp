#include "ADCS.h"

/**
 * @brief Construct a new ADCS::ADCS object
 *
 * @param i2c connected to the ADCS
 */
ADCS::ADCS(I2C & i2c) : i2c(i2c) {}

/**
 * @brief Destroy the ADCS::ADCS object
 *
 */
ADCS::~ADCS() {}

/**
 * @brief Initialize the ADCS
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t ADCS::initialize() {
  return MBED_ERROR_UNSUPPORTED;
}