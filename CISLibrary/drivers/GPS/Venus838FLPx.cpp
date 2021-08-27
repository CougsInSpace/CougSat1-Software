#include "Venus838FLPx.h"

/**
 * @brief Construct a new Venus838FLPx::Venus838FLPx object
 *
 * @param serial connected to the GPS
 * @param reset pin of the GPS
 * @param pulse pin of the GPS
 */
Venus838FLPx::Venus838FLPx(BufferedSerial & serial, PinName reset, PinName pulse) :
  serial(serial), reset(reset), pulse(pulse) {}

/**
 * @brief Destroy the Venus838FLPx::Venus838FLPx object
 *
 */
Venus838FLPx::~Venus838FLPx() {}

/**
 * @brief Read a GPS message and update fields
 *
 * @param data struct to return result
 * @param blocking will wait until data is present if true
 * @return mbed_error_status_t
 */
mbed_error_status_t read(GPSData_t & data, bool blocking = true) {
  return MBED_ERROR_UNSUPPORTED;
}