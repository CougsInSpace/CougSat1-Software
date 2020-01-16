#include "COMMS.h"

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
void __stdcall Comms::callback(const char * id, const char * value) {
  if (Hash::calculateHash(id) != Hash::calculateHash("main"))
    return spdlog::warn("Unknown Comms callback {} is {}", id, value);
  try {
    sendUpdate();
  } catch (const std::exception & e) {
    spdlog::error("Comms::callback onLoad failed: {}", e.what());
  }
}

/**
 * @brief Send an update to the page
 * Likely send real time values
 *
 */
void Comms::sendUpdate() {
  EBOutput_t output;
  EBError_t  error = EBCreateOutput("/cougsat-1/comms/", &output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  // TODO get real values
  EBAddOutputEx(output, "comms-status-700", "innerHTML", "TX - Telemetry");
  EBAddOutputEx(output, "antenna-700", "innerHTML", "Deployed");
  EBAddOutputEx(output, "comms-usage-700", "num", 1238057);
  EBAddOutputEx(output, "tx-power-700", "num", 26.5234);
  EBAddOutputEx(output, "rx-power", "num", -41.123);
  EBAddOutputEx(output, "snr", "num", 10.32);
  EBAddOutputEx(output, "ber", "num", 1.543e-5);
  EBAddOutputEx(output, "enqueued-segments-700", "num", 14);

  EBAddOutputEx(output, "comms-status-230", "innerHTML", "Idle");
  EBAddOutputEx(output, "antenna-230", "innerHTML", "Deployed");
  EBAddOutputEx(output, "comms-usage-230", "num", 154238057);
  EBAddOutputEx(output, "tx-power-230", "num", -10.5234);
  EBAddOutputEx(output, "enqueued-segments-230", "num", 1);

  error = EBEnqueueOutput(output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));
}

} // namespace CougSat1
} // namespace GUI