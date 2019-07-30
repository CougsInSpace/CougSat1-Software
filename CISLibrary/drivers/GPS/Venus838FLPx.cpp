#include "Venus838FLPx.h"

/**
 * @brief Construct a new Venus838FLPx::Venus838FLPx object
 *
 * @param serial connected to the GPS
 * @param reset pin of the GPS
 * @param pulse pin of the GPS
 */
Venus838FLPx::Venus838FLPx(Serial & serial, PinName reset, PinName pulse) :
  serial(serial), reset(reset), pulse(pulse) {}

/**
 * @brief Destroy the Venus838FLPx::Venus838FLPx object
 *
 */
Venus838FLPx::~Venus838FLPx() {}

/**
 * @brief Read a Venus838FLPx message and update fields
 *
 * @param blocking will wait until data is present
 * @return CISResult_t error code
 */
CISResult_t Venus838FLPx::update(bool blocking = true) {
  return {ERROR_NOT_SUPPORTED, ""};
}