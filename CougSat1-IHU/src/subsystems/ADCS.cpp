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
 * @brief Stores data from the ADCS
 * 
 * @param command CommandID
 * @param rx_buffer Response Buffer
 * @param buffer_size Length in Bytes
 * @return mbed_error_status_t 
 */
mbed_error_status_t ADCS::receiveBuffer(int8_t command, char* rx_buffer, size_t buffer_size) {
  switch (command) {
  case ADCS_CMD_LOCATION_REQ:
    BufferToArray<int32_t>(location, 2, rx_buffer, buffer_size);
    break;

  case ADCS_CMD_ORIENTATION_REQ:
    BufferToArray<int16_t>(orientation, 3, rx_buffer, buffer_size);
    break;

  case ADCS_CMD_TEMPERATURE_REQ:
    BufferToArray<int8_t>(coil_temperature, 3, rx_buffer, buffer_size);
    adcs_temperature = BufferToInt<int8_t>(rx_buffer, buffer_size, 3);
    gps_temperature = BufferToInt<int8_t>(rx_buffer, buffer_size, 4);
    break;

  case ADCS_CMD_COIL_CONTROL_REQ:
    BufferToArray<int8_t>(coil_control, 3, rx_buffer, buffer_size);
    break;

  case ADCS_CMD_COIL_CURRENT_REQ:
    BufferToArray<int8_t>(coil_current, 3, rx_buffer, buffer_size);
    break;

  default:
    return MBED_ERROR_EBADMSG;
  }

  return MBED_SUCCESS;
}


/**
 * @brief Initialize the ADCS
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t ADCS::initialize() {
  return MBED_ERROR_UNSUPPORTED;
}