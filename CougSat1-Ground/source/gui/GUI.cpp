#include "GUI.h"

#include <spdlog/spdlog.h>

namespace GUI {

/**
 * @brief Destroy the GUI::GUI object
 *
 */
GUI::~GUI() {}

/**
 * @brief Handle the input from the GUI
 *
 * @param msg to handle
 * @return Result
 */
Result GUI::handleInput(const EBMessage_t & msg) {
  spdlog::info("%s|%s|%s", msg.href.getString(), msg.id.getString(),
      msg.value.getString());
  return ResultCode_t::SUCCESS;
}

} // namespace GUI