#include "PMIC.h"

/**
 * @brief Construct a new PMIC::PMIC object
 *
 * @param i2c connected to the PMIC
 */
PMIC::PMIC(I2C & i2c) : i2c(i2c) {}

/**
 * @brief Destroy the PMIC::PMIC object
 *
 */
PMIC::~PMIC() {}

/**
 * @brief Initialize the PMIC
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t PMIC::initialize() {
  return MBED_ERROR_UNSUPPORTED;
}