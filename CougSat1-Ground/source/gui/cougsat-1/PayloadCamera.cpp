#include "PayloadCamera.h"

#include "Hash.h"

#include <spdlog/spdlog.h>

namespace GUI {
namespace CougSat1 {

/**
 * @brief Process input from the GUI
 *
 * @param id of the triggering element
 * @param value of the triggering element
 */
void __stdcall PayloadCamera::callback(const char * id, const char * value) {
  if (Hash::calculateHash(id) != Hash::calculateHash("main"))
    return spdlog::warn("Unknown PayloadCamera callback {} is {}", id, value);
  try {
    return spdlog::warn("TODO implement PayloadCamera::onLoad");
  } catch (const std::exception & e) {
    spdlog::error("PayloadCamera::callback onLoad failed: {}", e.what());
  }
}

} // namespace CougSat1
} // namespace GUI