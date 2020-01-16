#include "ADCS.h"

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
void __stdcall ADCS::callback(const char * id, const char * value) {
  if (Hash::calculateHash(id) != Hash::calculateHash("main"))
    return spdlog::warn("Unknown ADCS callback {} is {}", id, value);
  try {
    return sendUpdate();
  } catch (const std::exception & e) {
    spdlog::error("ADCS::callback onLoad failed: {}", e.what());
  }
}

/**
 * @brief Send an update to the page
 * Likely send real time values
 *
 */
void ADCS::sendUpdate() {
  EBOutput_t output;
  EBError_t  error = EBCreateOutput("/cougsat-1/adcs/", &output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  // TODO get real values
  EBAddOutputEx(output, "adcs-status", "innerHTML", "Target locked");
  EBAddOutputEx(output, "adcs-target", "innerHTML", "Pullman");
  EBAddOutputEx(output, "lat", "num", -124.3213);
  EBAddOutputEx(output, "long", "num", 42.4321);
  EBAddOutputEx(output, "alt", "num", 400.12345e3);
  EBAddOutputEx(output, "roll", "num", (rand() % 3600) / 10.0);
  EBAddOutputEx(output, "pitch", "num", (rand() % 3600) / 10.0);
  EBAddOutputEx(output, "yaw", "num", (rand() % 3600) / 10.0);
  EBAddOutputEx(output, "velocity-linear", "num", 8032.123);
  EBAddOutputEx(output, "velocity-angular", "num", 1.3234);
  EBAddOutputEx(output, "magnetic-field", "num", 1.3234e-4);
  EBAddOutputEx(output, "gravity", "num", 9.806);
  EBAddOutputEx(output, "magnet-x", "num", 0.3 * (rand() % 10000) / 10000.0);
  EBAddOutputEx(output, "magnet-y", "num", 0.3 * (rand() % 10000) / 10000.0);
  EBAddOutputEx(output, "magnet-z", "num", 0.3 * (rand() % 10000) / 10000.0);
  EBAddOutputEx(output, "map", "TLE",
      "1 25544U 98067A   19272.17332272  .00000706  00000-0  20336-4 0  9999"
      "2 25544  51.6426 205.5801 0007437  95.6316 228.4969 15.50113298191385");

  error = EBEnqueueOutput(output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));
}

} // namespace CougSat1
} // namespace GUI