#include "CougSat1.h"

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
void __stdcall CougSat1::callback(const char * id, const char * value) {
  if (Hash::calculateHash(id) != Hash::calculateHash("main"))
    return spdlog::warn("Unknown CougSat-1 callback {} is {}", id, value);
  try {
    return sendUpdate();
  } catch (const std::exception & e) {
    spdlog::error("CougSat1::callback backup sendUpdate failed: {}", e.what());
  }
}

/**
 * @brief Send an update to the page
 * Likely send real time values
 *
 */
void CougSat1::sendUpdate() {
  EBOutput_t output;
  EBError_t  error = EBCreateOutput("/cougsat-1/", &output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  // TODO get real values
  // ADCS
  EBAddOutputEx(output, "adcs-status", "innerHTML", "Target locked");
  EBAddOutputEx(output, "adcs-target", "innerHTML", "Pullman");
  EBAddOutputEx(output, "lat", "num", -124.3213);
  EBAddOutputEx(output, "long", "num", 42.4321);

  // Comms
  EBAddOutputEx(output, "comms-status", "innerHTML", "TX - Telemetry");
  EBAddOutputEx(output, "rx-power", "num", -41.123);
  EBAddOutputEx(output, "snr", "num", 10.32);
  EBAddOutputEx(output, "ber", "num", 1.543e-5);
  EBAddOutputEx(output, "tx-power-700", "num", 26.5234);
  EBAddOutputEx(output, "tx-power-230", "num", -10.5234);
  EBAddOutputEx(output, "enqueued-segments-700", "num", 14);
  EBAddOutputEx(output, "enqueued-segments-230", "num", 1);

  // EPS
  EBAddOutputEx(output, "eps-status", "innerHTML", "Nominal - Discharging");
  EBAddOutputEx(output, "main", "PV_CELL_0A_V", 2.11231);
  EBAddOutputEx(output, "main", "PV_CELL_0A_I", 0.27085);
  EBAddOutputEx(output, "main", "PV_CELL_0B_V", 2.11231);
  EBAddOutputEx(output, "main", "PV_CELL_0B_I", 0.27085);
  EBAddOutputEx(output, "main", "PV_CELL_1A_V", 2.11231);
  EBAddOutputEx(output, "main", "PV_CELL_1A_I", 0.27085);
  EBAddOutputEx(output, "main", "PV_CELL_1B_V", 2.11231);
  EBAddOutputEx(output, "main", "PV_CELL_1B_I", 0.27085);
  EBAddOutputEx(output, "main", "PV_CELL_2A_V", 1.11231);
  EBAddOutputEx(output, "main", "PV_CELL_2A_I", 0.06484);
  EBAddOutputEx(output, "main", "PV_CELL_2B_V", 1.11231);
  EBAddOutputEx(output, "main", "PV_CELL_2B_I", 0.06484);
  EBAddOutputEx(output, "main", "PV_CELL_3A_V", 1.11231);
  EBAddOutputEx(output, "main", "PV_CELL_3A_I", 0.06484);
  EBAddOutputEx(output, "main", "PV_CELL_3B_V", 1.11231);
  EBAddOutputEx(output, "main", "PV_CELL_3B_I", 0.06484);
  EBAddOutputEx(output, "main", "BATT_IN_A_I", 0.38179);
  EBAddOutputEx(output, "main", "BATT_IN_B_I", 0.25452);
  EBAddOutputEx(output, "main", "BATT_OUT_A_I", 0.39413);
  EBAddOutputEx(output, "main", "BATT_OUT_B_I", 0.09218);
  EBAddOutputEx(output, "main", "BATT_A_V", 3.7123);
  EBAddOutputEx(output, "main", "BATT_B_V", 3.7123);
  EBAddOutputEx(output, "main", "BATT_A_C", 1.9132);
  EBAddOutputEx(output, "main", "BATT_B_C", 1.23341);
  EBAddOutputEx(output, "main", "PR_BH_0_I", 0.0001123);
  EBAddOutputEx(output, "main", "PR_BH_1_I", 0.012309);
  EBAddOutputEx(output, "main", "PR_DEPLOY_0_I", 0.0);
  EBAddOutputEx(output, "main", "PR_DEPLOY_1_I", 0.0);
  EBAddOutputEx(output, "main", "PR_BATT_0_I", 0.1087);
  EBAddOutputEx(output, "main", "PR_BATT_1_I", 0.0);
  EBAddOutputEx(output, "main", "PR_BATT_2_I", 0.0);
  EBAddOutputEx(output, "main", "PR_BATT_3_I", 0.0);
  EBAddOutputEx(output, "main", "PR_BATT_4_I", 0.0);
  EBAddOutputEx(output, "main", "PR_BATT_5_I", 0.03452);
  EBAddOutputEx(output, "main", "PR_BATT_6_I", 0.20067);
  EBAddOutputEx(output, "main", "REG_OUT_A_V", 3.2934);
  EBAddOutputEx(output, "main", "REG_OUT_B_V", 3.2932);
  EBAddOutputEx(output, "main", "PR_3V3_0_I", 0.01432);
  EBAddOutputEx(output, "main", "PR_3V3_1_I", 0.0);
  EBAddOutputEx(output, "main", "PR_3V3_2_I", 0.03210);
  EBAddOutputEx(output, "main", "PR_3V3_3_I", 0.0);
  EBAddOutputEx(output, "main", "PR_3V3_4_I", 0.0);
  EBAddOutputEx(output, "main", "PR_3V3_5_I", 0.0);
  EBAddOutputEx(output, "main", "PR_3V3_6_I", 0.04123);
  EBAddOutputEx(output, "main", "PR_3V3_7_I", 0.0);
  EBAddOutputEx(output, "main", "PR_3V3_8_I", 0.0);
  EBAddOutputEx(output, "main", "PR_3V3_9_I", 0.0);
  EBAddOutputEx(output, "main", "PR_3V3_10_I", 0.004170);
  EBAddOutputEx(output, "main", "PR_3V3_11_I", 0.0);
  EBAddOutputEx(output, "main", "PR_3V3_12_I", 0.0);

  // ECS
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