#include "IFJR.h"

/**
 * @brief Construct a new IFJR::IFJR object
 *
 * @param i2c connected to the IFJR
 */
IFJR::IFJR(I2C & i2c) : i2c(i2c) {}

/**
 * @brief Destroy the IFJR::IFJR object
 *
 */
IFJR::~IFJR() {}

/**
 * @brief Initialize the IFJR
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t IFJR::initialize() {
  return MBED_ERROR_UNSUPPORTED;
}