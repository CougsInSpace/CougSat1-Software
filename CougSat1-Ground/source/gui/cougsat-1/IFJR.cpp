#include "IFJR.h"

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
void __stdcall IFJR::callback(const char * id, const char * value) {
  try {
    switch (Hash::calculateHash(id)) {
      case Hash::calculateHash("main"):
        return sendUpdate();
      default:
        return spdlog::warn("Unknown IFJR callback {} is {}", id, value);
    }
  } catch (const std::exception & e) {
    spdlog::error("IFJR::callback encountered: {}", e.what());
    try {
      sendUpdate(); // Synchronize GUI
    } catch (const std::exception & e) {
      spdlog::error("IFJR::callback backup sendUpdate failed: {}", e.what());
    }
  }
}

/**
 * @brief Send an update to the page
 * Likely send real time values
 *
 */
void IFJR::sendUpdate() {
  EBOutput_t output;
  EBError_t  error = EBCreateOutput("/cougsat-1/ifjr/", &output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  // TODO get real values
  EBAddOutputEx(output, "ifjr-status", "innerHTML", "Idle");

  error = EBEnqueueOutput(output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));
}

} // namespace CougSat1
} // namespace GUI