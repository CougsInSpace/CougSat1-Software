#include "CougSat1.h"

#include <spdlog/spdlog.h>

namespace GUI {

/**
 * @brief Construct a new CougSat1: CougSat1 object
 *
 * @param gui
 */
CougSat1::CougSat1(EBGUI_t gui) : Page(gui, "") {}

/**
 * @brief Destroy the CougSat1: CougSat1 object
 *
 */
CougSat1::~CougSat1() {}

/**
 * @brief Handle when the page first loads
 * Likely send initial values
 *
 * @return Result
 */
Result CougSat1::onLoad() {
  try {
    createNewEBMessage();

    enqueueEBMessage();
  } catch (const std::exception & e) {
    return ResultCode_t::EXCEPTION_OCCURRED + e.what();
  }

  return sendUpdate();
}

/**
 * @brief Send an update to the page
 * Likely send real time values
 *
 * @return Result
 */
Result CougSat1::sendUpdate() {
  try {
    createNewEBMessage();

    // TODO get real numbers
    // ADCS
    messageSetProp("adcs-status", "innerHTML", "Target locked");
    messageSetProp("adcs-target", "innerHTML", "Pullman");
    messageSetProp("lat", "num", -124.3213);
    messageSetProp("long", "num", 42.4321);
    messageSetProp("alt", "num", 400.12345e3);
    messageSetProp("roll", "num", (rand() % 3600) / 10.0);
    messageSetProp("pitch", "num", (rand() % 3600) / 10.0);
    messageSetProp("yaw", "num", (rand() % 3600) / 10.0);
    messageSetProp("velocity-linear", "num", 8032.123);
    messageSetProp("velocity-angular", "num", 1.3234);
    messageSetProp("magnetic-field", "num", 1.3234e-4);
    messageSetProp("gravity", "num", 9.806);
    messageSetProp("magnet-x", "num", 0.3 * (rand() % 10000) / 10000.0);
    messageSetProp("magnet-y", "num", 0.3 * (rand() % 10000) / 10000.0);
    messageSetProp("magnet-z", "num", 0.3 * (rand() % 10000) / 10000.0);
    messageSetProp("map", "TLE",
        "1 25544U 98067A   19272.17332272  .00000706  00000-0  20336-4 0  9999"
        "2 25544  51.6426 205.5801 0007437  95.6316 228.4969 "
        "15.50113298191385");

    // C&DH
    messageSetProp("cdh-status", "innerHTML", "Idle");
    messageSetProp("events-enqueued", "num", (int64_t)5);
    messageSetProp("cdh-storage", "num", (int64_t)54315345);

    // Comms
    messageSetProp("comms-status", "innerHTML", "TX - Telemetry");

    messageSetProp("comms-status-700", "innerHTML", "TX - Telemetry");
    messageSetProp("antenna-700", "innerHTML", "Deployed");
    messageSetProp("comms-usage-700", "num", (int64_t)1238057);
    messageSetProp("tx-power-700", "num", 26.5234);
    messageSetProp("rx-power", "num", -41.123);
    messageSetProp("snr", "num", 10.32);
    messageSetProp("ber", "num", 1.543e-5);
    messageSetProp("enqueued-segments-700", "num", (int64_t)14);

    messageSetProp("comms-status-230", "innerHTML", "Idle");
    messageSetProp("antenna-230", "innerHTML", "Deployed");
    messageSetProp("comms-usage-230", "num", (int64_t)154238057);
    messageSetProp("tx-power-230", "num", -10.5234);
    messageSetProp("enqueued-segments-230", "num", (int64_t)1);

    // EPS
    messageSetProp("eps-status", "innerHTML", "Nominal - Discharging");
    messageSetProp("main", "PV_CELL_0A_V", 2.11231);
    messageSetProp("main", "PV_CELL_0A_I", 0.27085);
    messageSetProp("main", "PV_CELL_0B_V", 2.11231);
    messageSetProp("main", "PV_CELL_0B_I", 0.27085);
    messageSetProp("main", "PV_CELL_1A_V", 2.11231);
    messageSetProp("main", "PV_CELL_1A_I", 0.27085);
    messageSetProp("main", "PV_CELL_1B_V", 2.11231);
    messageSetProp("main", "PV_CELL_1B_I", 0.27085);
    messageSetProp("main", "PV_CELL_2A_V", 1.11231);
    messageSetProp("main", "PV_CELL_2A_I", 0.06484);
    messageSetProp("main", "PV_CELL_2B_V", 1.11231);
    messageSetProp("main", "PV_CELL_2B_I", 0.06484);
    messageSetProp("main", "PV_CELL_3A_V", 1.11231);
    messageSetProp("main", "PV_CELL_3A_I", 0.06484);
    messageSetProp("main", "PV_CELL_3B_V", 1.11231);
    messageSetProp("main", "PV_CELL_3B_I", 0.06484);
    messageSetProp("main", "PV_IN_0A_V", 3.7123);
    messageSetProp("main", "PV_IN_0A_I", 0.14949);
    messageSetProp("main", "PV_IN_0B_V", 3.7123);
    messageSetProp("main", "PV_IN_0B_I", 0.14949);
    messageSetProp("main", "PV_IN_1A_V", 3.7123);
    messageSetProp("main", "PV_IN_1A_I", 0.14949);
    messageSetProp("main", "PV_IN_1B_V", 3.7123);
    messageSetProp("main", "PV_IN_1B_I", 0.14949);
    messageSetProp("main", "PV_IN_2A_V", 3.7123);
    messageSetProp("main", "PV_IN_2A_I", 0.012457);
    messageSetProp("main", "PV_IN_2B_V", 3.7123);
    messageSetProp("main", "PV_IN_2B_I", 0.012457);
    messageSetProp("main", "PV_IN_3A_V", 3.7123);
    messageSetProp("main", "PV_IN_3A_I", 0.012457);
    messageSetProp("main", "PV_IN_3B_V", 3.7123);
    messageSetProp("main", "PV_IN_3B_I", 0.012457);
    messageSetProp("main", "BATT_IN_A_I", 0.38179);
    messageSetProp("main", "BATT_IN_B_I", 0.25452);
    messageSetProp("main", "BATT_OUT_A_I", 0.39413);
    messageSetProp("main", "BATT_OUT_B_I", 0.09218);
    messageSetProp("main", "BATT_A_V", 3.7123);
    messageSetProp("main", "BATT_B_V", 3.7123);
    messageSetProp("main", "BATT_A_C", 1.9132);
    messageSetProp("main", "BATT_B_C", 1.23341);
    messageSetProp("main", "PR_BH_0_I", 0.0001123);
    messageSetProp("main", "PR_BH_1_I", 0.012309);
    messageSetProp("main", "PR_DEPLOY_0_I", 0.0);
    messageSetProp("main", "PR_DEPLOY_1_I", 0.0);
    messageSetProp("main", "PR_BATT_0_I", 0.1087);
    messageSetProp("main", "PR_BATT_1_I", 0.0);
    messageSetProp("main", "PR_BATT_2_I", 0.0);
    messageSetProp("main", "PR_BATT_3_I", 0.0);
    messageSetProp("main", "PR_BATT_4_I", 0.0);
    messageSetProp("main", "PR_BATT_5_I", 0.03452);
    messageSetProp("main", "PR_BATT_6_I", 0.20067);
    messageSetProp("main", "REG_IN_A_I", 0.06321);
    messageSetProp("main", "REG_IN_B_I", 0.06071);
    messageSetProp("main", "REG_OUT_A_V", 3.2934);
    messageSetProp("main", "REG_OUT_B_V", 3.2932);
    messageSetProp("main", "REG_OUT_A_I", 0.07080);
    messageSetProp("main", "REG_OUT_B_I", 0.065984);
    messageSetProp("main", "PR_3V3_PV_I", 0.00431);
    messageSetProp("main", "PR_3V3_EPS_I", 0.0231);
    messageSetProp("main", "PR_3V3_0_I", 0.01432);
    messageSetProp("main", "PR_3V3_1_I", 0.0);
    messageSetProp("main", "PR_3V3_2_I", 0.03210);
    messageSetProp("main", "PR_3V3_3_I", 0.0);
    messageSetProp("main", "PR_3V3_4_I", 0.0);
    messageSetProp("main", "PR_3V3_5_I", 0.0);
    messageSetProp("main", "PR_3V3_6_I", 0.04123);
    messageSetProp("main", "PR_3V3_7_I", 0.0);
    messageSetProp("main", "PR_3V3_8_I", 0.0);
    messageSetProp("main", "PR_3V3_9_I", 0.0);
    messageSetProp("main", "PR_3V3_10_I", 0.004170);
    messageSetProp("main", "PR_3V3_11_I", 0.0);
    messageSetProp("main", "PR_3V3_12_I", 0.0);

    // ECS
    messageSetProp("ecs-status", "innerHTML", "Subsystems overheating");
    messageSetProp("main", "TEMP_ADCS", -20.112);
    messageSetProp("main", "TEMP_CDH", 0.112);
    messageSetProp("main", "TEMP_COMMS", 20.112);
    messageSetProp("main", "TEMP_EPS", -40.112);
    messageSetProp("main", "TEMP_BATT_A", 60.112);
    messageSetProp("main", "TEMP_BATT_B", 70.112);
    messageSetProp("main", "TEMP_PX", 85.112);
    messageSetProp("main", "TEMP_NX", 90.112);
    messageSetProp("main", "TEMP_PY", -47.112);
    messageSetProp("main", "TEMP_NY", -20.112);
    messageSetProp("main", "TEMP_PZ", -20.112);
    messageSetProp("main", "TEMP_NZ", -20.112);
    messageSetProp("main", "TEMP_PLANT", -20.112);

    // IFJR
    messageSetProp("ifjr-status", "innerHTML", "Idle");

    enqueueEBMessage();
  } catch (const std::exception & e) {
    return ResultCode_t::EXCEPTION_OCCURRED + e.what();
  }
  return ResultCode_t::SUCCESS;
}

/**
 * @brief Handle the input from the GUI
 *
 * @param msg to handle
 * @return Result
 */
Result CougSat1::handleInput(const EBMessage_t & msg) {
  try {
    createNewEBMessage();
    switch (msg.id.get()) {
      case Hash::calculateHash("main"):
        return onLoad();
      case Hash::calculateHash("event-queue-request"):
      case Hash::calculateHash("event-queue-update"):
      case Hash::calculateHash("image-input"):
      case Hash::calculateHash("ifjr-target"):
      case Hash::calculateHash("ifjr-time"):
      case Hash::calculateHash("ifjr-enqueue"):
        // TODO implement
        break;
      default:
        spdlog::warn("Unknown input for " + msg.href.getString());
    }

    enqueueEBMessage();
  } catch (const std::exception & e) {
    return ResultCode_t::EXCEPTION_OCCURRED + e.what();
  }
  return ResultCode_t::SUCCESS;
}

} // namespace GUI