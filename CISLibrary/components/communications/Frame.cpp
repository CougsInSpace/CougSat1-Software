#include "Frame.h"

namespace Communications {

/**
 * @brief Construct a new Frame:: Frame object
 *
 */
Frame::Frame() {}

/**
 * @brief Destroy the Frame:: Frame object
 *
 */
Frame::~Frame() {}

/**
 * @brief Add a byte to the frame from the receiver
 *
 *
 * @param byte
 */
void Frame::add(uint8_t byte) {}

/**
 * @brief Check if the frame is complete
 *
 * @return true when the EOF has been received or when the EOF has been got in
 * getNextTXByte
 * @return false otherwise
 */
bool Frame::isDone() {
  return false;
}

/**
 * @brief Get the next byte to transmit
 *
 * @return uint8_t
 */
uint8_t Frame::getNextTXByte() {
  return -1;
}

} // namespace Communications