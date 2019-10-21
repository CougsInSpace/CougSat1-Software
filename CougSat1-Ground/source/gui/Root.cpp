#include "Root.h"

#include <spdlog/spdlog.h>

namespace GUI {

/**
 * @brief Construct a new Root:: Root object
 *
 * @param gui
 */
Root::Root(EBGUI_t gui) : Page(gui, "") {}

/**
 * @brief Destroy the Root:: Root object
 *
 */
Root::~Root() {}

/**
 * @brief Handle when the page first loads
 * Likely send initial values
 *
 * @return Result
 */
Result Root::onLoad() {
  try {
    createNewEBMessage();

    enqueueEBMessage();
  } catch (const std::exception & e) {
    return ResultCode_t::EXCEPTION_OCCURRED + e.what();
  }

  return sendUpdate();
}

/**
 * @brief Send an update to the page
 * Likely send real time values
 *
 * @return Result
 */
Result Root::sendUpdate() {
  try {
    createNewEBMessage();

    enqueueEBMessage();
  } catch (const std::exception & e) {
    return ResultCode_t::EXCEPTION_OCCURRED + e.what();
  }
  return ResultCode_t::SUCCESS;
}

/**
 * @brief Handle the input from the GUI
 *
 * @param msg to handle
 * @return Result
 */
Result Root::handleInput(const EBMessage_t & msg) {
  try {
    createNewEBMessage();

    switch (msg.id.get()) {
      case Hash::calculateHash("upload"):
      case Hash::calculateHash("name"):
      case Hash::calculateHash("lat"):
      case Hash::calculateHash("long"):
      case Hash::calculateHash("grid"):
      case Hash::calculateHash("alt"):
      case Hash::calculateHash("desc"):
        // TODO implement settings changes
        break;
      default:
        spdlog::info("Unknown id for Root");
        break;
    }

    enqueueEBMessage();
  } catch (const std::exception & e) {
    return ResultCode_t::EXCEPTION_OCCURRED + e.what();
  }
  return ResultCode_t::SUCCESS;
}

} // namespace GUI