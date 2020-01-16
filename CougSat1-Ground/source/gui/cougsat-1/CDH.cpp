#include "CDH.h"

#include "Hash.h"

#include <Ehbanana.h>
#include <spdlog/spdlog.h>

namespace GUI {
namespace CougSat1 {

/**
 * @brief Process input from the GUI
 *
 * @param id of the triggering element
 * @param value of the triggering element
 */
void __stdcall CDH::callback(const char * id, const char * value) {
  try {
    switch (Hash::calculateHash(id)) {
      case Hash::calculateHash("main"):
        return sendUpdate();
      default:
        return spdlog::warn("Unknown CDH callback {} is {}", id, value);
    }
  } catch (const std::exception & e) {
    spdlog::error("CDH::callback encountered: {}", e.what());
    try {
      sendUpdate(); // Synchronize GUI
    } catch (const std::exception & e) {
      spdlog::error("CDH::callback backup sendUpdate failed: {}", e.what());
    }
  }
}

/**
 * @brief Send an update to the page
 * Likely send real time values
 *
 */
void CDH::sendUpdate() {
  EBOutput_t output;
  EBError_t  error = EBCreateOutput("/cougsat-1/c&dh/", &output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  // TODO get real values
  EBAddOutputEx(output, "cdh-status", "innerHTML", "Idle");
  EBAddOutputEx(output, "events-enqueued", "num", 5);
  EBAddOutputEx(output, "cdh-storage", "num", 54315345);

  error = EBEnqueueOutput(output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));
}

} // namespace CougSat1
} // namespace GUI