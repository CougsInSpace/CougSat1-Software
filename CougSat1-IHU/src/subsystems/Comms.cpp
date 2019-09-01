#include "Comms.h"

/**
 * @brief Construct a new Comms::Comms object
 *
 * @param spi connected to the Comms
 */
Comms::Comms(SPI & spi) : spi(spi) {}

/**
 * @brief Destroy the Comms::Comms object
 *
 */
Comms::~Comms() {}

/**
 * @brief Initialize the Comms
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t Comms::initialize() {
  return MBED_ERROR_UNSUPPORTED;
}