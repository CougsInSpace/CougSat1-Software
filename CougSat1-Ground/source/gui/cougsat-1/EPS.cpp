#include "EPS.h"

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
void __stdcall EPS::callback(const char * id, const char * value) {
  if (Hash::calculateHash(id) != Hash::calculateHash("main"))
    return spdlog::warn("Unknown EPS callback {} is {}", id, value);
  try {
    return sendUpdate();
  } catch (const std::exception & e) {
    spdlog::error("EPS::callback onLoad failed: {}", e.what());
  }
}

/**
 * @brief Send an update to the page
 * Likely send real time values
 *
 */
void EPS::sendUpdate() {
  EBOutput_t output;
  EBError_t  error = EBCreateOutput("/cougsat-1/eps/", &output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  // TODO get real values
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
  EBAddOutputEx(output, "main", "PV_IN_0A_V", 3.7123);
  EBAddOutputEx(output, "main", "PV_IN_0A_I", 0.14949);
  EBAddOutputEx(output, "main", "PV_IN_0B_V", 3.7123);
  EBAddOutputEx(output, "main", "PV_IN_0B_I", 0.14949);
  EBAddOutputEx(output, "main", "PV_IN_1A_V", 3.7123);
  EBAddOutputEx(output, "main", "PV_IN_1A_I", 0.14949);
  EBAddOutputEx(output, "main", "PV_IN_1B_V", 3.7123);
  EBAddOutputEx(output, "main", "PV_IN_1B_I", 0.14949);
  EBAddOutputEx(output, "main", "PV_IN_2A_V", 3.7123);
  EBAddOutputEx(output, "main", "PV_IN_2A_I", 0.012457);
  EBAddOutputEx(output, "main", "PV_IN_2B_V", 3.7123);
  EBAddOutputEx(output, "main", "PV_IN_2B_I", 0.012457);
  EBAddOutputEx(output, "main", "PV_IN_3A_V", 3.7123);
  EBAddOutputEx(output, "main", "PV_IN_3A_I", 0.012457);
  EBAddOutputEx(output, "main", "PV_IN_3B_V", 3.7123);
  EBAddOutputEx(output, "main", "PV_IN_3B_I", 0.012457);
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
  EBAddOutputEx(output, "main", "REG_IN_A_I", 0.06321);
  EBAddOutputEx(output, "main", "REG_IN_B_I", 0.06071);
  EBAddOutputEx(output, "main", "REG_OUT_A_V", 3.2934);
  EBAddOutputEx(output, "main", "REG_OUT_B_V", 3.2932);
  EBAddOutputEx(output, "main", "REG_OUT_A_I", 0.07080);
  EBAddOutputEx(output, "main", "REG_OUT_B_I", 0.065984);
  EBAddOutputEx(output, "main", "PR_3V3_PV_I", 0.00431);
  EBAddOutputEx(output, "main", "PR_3V3_EPS_I", 0.0231);
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

  error = EBEnqueueOutput(output);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));
}

} // namespace CougSat1
} // namespace GUI