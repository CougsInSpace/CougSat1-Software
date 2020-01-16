#include "Radio.h"

#include "Hash.h"

#include <Ehbanana.h>
#include <spdlog/spdlog.h>

namespace GUI {

/**
 * @brief Process input from the GUI
 *
 * @param id of the triggering element
 * @param value of the triggering element
 */
void __stdcall Radio::callback(const char * id, const char * value) {
  try {
    switch (Hash::calculateHash(id)) {
      case Hash::calculateHash("main"):
        return sendUpdate();
      case Hash::calculateHash("rx-source"):
        return changeRXSource(value);
      case Hash::calculateHash("tx-destination"):
        return spdlog::warn("TODO implement Radio settings changes");
      default:
        return spdlog::warn("Unknown Root callback {} is {}", id, value);
    }
  } catch (const std::exception & e) {
    spdlog::error("Radio::callback encountered: {}", e.what());
    try {
      // sendUpdate();// Synchronize GUI TODO
    } catch (const std::exception & e) {
      spdlog::error("Radio::callback backup sendUpdate failed: {}", e.what());
    }
  }
}

/**
 * @brief Change the RX source of the radio
 *
 * @param value
 */
void Radio::changeRXSource(const char * value) {
  switch (Hash::calculateHash(value)) {
    case Hash::calculateHash("None"):
    case Hash::calculateHash("RTL-SDR"):
    case Hash::calculateHash("Audio"):
    case Hash::calculateHash("IQ File"):
    case Hash::calculateHash("Test Telemetry"):
    default:
      return spdlog::warn("Unknown changeRXSource: {}", value);
  }
}

/**
 * @brief Send an update to the page
 * Likely send real time values
 *
 */
void Radio::sendUpdate() {
  EBOutput_t output;
  EBError_t  error = EBCreateOutput("/radio/", &output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  // TODO get real values
  EBAddOutputEx(output, "link-status", "innerHTML", "Connected");
  EBAddOutputEx(output, "antenna-az", "num", 120.113);
  EBAddOutputEx(output, "antenna-el", "num", 20.73);
  EBAddOutputEx(output, "data-rate", "num", 2130.73);
  EBAddOutputEx(output, "aos", "num", 34.1);
  EBAddOutputEx(output, "los", "num", -547.1);

  EBAddOutputEx(output, "freq-offset", "num", 6821.1);
  EBAddOutputEx(output, "snr", "num", 8.1);
  EBAddOutputEx(output, "ber", "num", 21.13e-6);
  EBAddOutputEx(output, "frames", "num", 123);

  // TODO send rx and tx source config

  error = EBEnqueueOutput(output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));
}

} // namespace GUI