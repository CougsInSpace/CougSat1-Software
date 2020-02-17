#include "ECS.h"

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
void __stdcall ECS::callback(const char * id, const char * value) {
  if (Hash::calculateHash(id) != Hash::calculateHash("main"))
    return spdlog::warn("Unknown ECS callback {} is {}", id, value);
  try {
    sendUpdate();
  } catch (const std::exception & e) {
    spdlog::error("ECS::callback onLoad failed: {}", e.what());
  }
}

/**
 * @brief Send an update to the page
 * Likely send real time values
 *
 */
void ECS::sendUpdate() {
  EBOutput_t output;
  EBError_t  error = EBCreateOutput("/cougsat-1/ecs/", &output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  // TODO get real values
  EBAddOutputEx(output, "ecs-status", "innerHTML", "Subsystems overheating");
  EBAddOutputEx(output, "main", "TEMP_ADCS", -20.112);
  EBAddOutputEx(output, "main", "TEMP_CDH", 0.112);
  EBAddOutputEx(output, "main", "TEMP_COMMS", 20.112);
  EBAddOutputEx(output, "main", "TEMP_EPS", -40.112);
  EBAddOutputEx(output, "main", "TEMP_BATT_A", 60.112);
  EBAddOutputEx(output, "main", "TEMP_BATT_B", 70.112);
  EBAddOutputEx(output, "main", "TEMP_PX", 85.112);
  EBAddOutputEx(output, "main", "TEMP_NX", 90.112);
  EBAddOutputEx(output, "main", "TEMP_PY", -47.112);
  EBAddOutputEx(output, "main", "TEMP_NY", -20.112);
  EBAddOutputEx(output, "main", "TEMP_PZ", -20.112);
  EBAddOutputEx(output, "main", "TEMP_NZ", -20.112);
  EBAddOutputEx(output, "main", "TEMP_PLANT", -20.112);

  error = EBEnqueueOutput(output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));
}

} // namespace CougSat1
} // namespace GUI