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
 * @brief Request Location from ADCS
 * 
 * @return mbed_error_status_t 
 */
mbed_error_status_t ADCS::requestLocation() {
  if (i2c_busy) {
    return MBED_ERROR_DEVICE_BUSY;
  }

  i2c_busy = true;

  tx_buffer[0] = ADCS_CMD_LOCATION_REQ;

  transfer_complete_callback = event_callback_t(this, &ADCS::receiveLocation);

  int status = i2c.transfer(
    I2C_ADCS_ADDRESS,
    tx_buffer, I2C_DATA_LENGTH,
    rx_buffer, I2C_DATA_LENGTH,
    event_callback_t(this, &ADCS::receiveDataHandler),
    I2C_EVENT_ALL
  );

  return status == 0 ? MBED_SUCCESS : MBED_ERROR_DEVICE_BUSY;
}

/**
 * @brief Stores the location into the class.
 * 
 * @param status 
 */
void ADCS::receiveLocation (int status) {
  latitude  = Int32FromBuffer(rx_buffer, 0);
  longitude = Int32FromBuffer(rx_buffer, 4);
}

/**
 * @brief Returns the latitude, longitude.
 * 
 * @return std::tuple<int32_t, int32_t> 
 */
std::tuple<int32_t, int32_t> ADCS::getLocation() {
  return std::make_tuple(latitude, longitude);
}

/**
 * @brief Handles all errors, and only passes data with correct statuses
 * to the parsers.
 * 
 * @param status 
 */
void ADCS::receiveDataHandler(int status) {
  if (status & I2C_EVENT_ERROR) {
    //TODO: Handle the error somehow.
  }

  if (status & I2C_EVENT_TRANSFER_COMPLETE) {
    transfer_complete_callback.call(status);
    i2c_busy = false;
  }
}

/**
 * @brief Initialize the ADCS
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t ADCS::initialize() {
  return MBED_ERROR_UNSUPPORTED;
}