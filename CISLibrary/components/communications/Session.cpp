#include "Session.h"

namespace Communications {

/**
 * @brief Construct a new Session:: Session object
 *
 */
Session::Session() {}

/**
 * @brief Destroy the Session:: Session object
 *
 */
Session::~Session() {}

/**
 * @brief Add a frame to the session from the receiver
 *
 * @param frame
 */
void Session::add(std::unique_ptr<Frame> frame) {}

/**
 * @brief Get the session ID
 *
 * @return uint16_t
 */
uint16_t Session::getID() {
  return -1;
}

/**
 * @brief Check if the TX queue is empty
 *
 * @return true if there are no frames to transmit
 * @return false otherwise
 */
bool Session::isTXEmpty() {
  return true;
}

/**
 * @brief Get the next frame in the TX queue
 *
 * @return std::unique_ptr<Frame>
 */
std::unique_ptr<Frame> Session::getNextFrame() {
  return nullptr;
}

} // namespace Communications