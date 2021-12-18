#include "Root.h"

#include "Hash.h"

#include <spdlog/spdlog.h>

namespace GUI {

/**
 * @brief Process input from the GUI
 *
 * @param id of the triggering element
 * @param value of the triggering element
 */
void __stdcall Root::callback(const char * id, const char * value) {
  try {
    switch (Hash::calculateHash(id)) {
      case Hash::calculateHash("upload"):
      case Hash::calculateHash("name"):
      case Hash::calculateHash("lat"):
      case Hash::calculateHash("long"):
      case Hash::calculateHash("grid"):
      case Hash::calculateHash("alt"):
      case Hash::calculateHash("desc"):
        return spdlog::warn("TODO implement Root settings changes");
      default:
        return spdlog::warn("Unknown Root callback {} is {}", id, value);
    }
  } catch (const std::exception & e) {
    spdlog::error("Root::callback encountered: {}", e.what());
    try {
      // sendUpdate();// Synchronize GUI TODO
    } catch (const std::exception & e) {
      spdlog::error("Root::callback backup sendUpdate failed: {}", e.what());
    }
  }
}

} // namespace GUI