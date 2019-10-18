#include "EPS.h"

namespace GUI {
namespace CougSat1 {

/**
 * @brief Construct a new EPS:", EPS object
 *
 * @param gui
 */
EPS::EPS(EBGUI_t gui) : Page(gui, "/cougsat-1/eps/") {}

/**
 * @brief Destroy the EPS: EPS object
 *
 */
EPS::~EPS() {}

/**
 * @brief Handle when the page first loads
 * Likely send initial values
 *
 * @return Result
 */
Result EPS::onLoad() {
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
Result EPS::sendUpdate() {
  try {
    createNewEBMessage();

    // TODO get real numbers
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
Result EPS::handleInput(const EBMessage_t & msg) {
  try {
    createNewEBMessage();
    switch (msg.id.get()) {
      case Hash::calculateHash("main"):
        return onLoad();
    }

    enqueueEBMessage();
  } catch (const std::exception & e) {
    return ResultCode_t::EXCEPTION_OCCURRED + e.what();
  }
  return ResultCode_t::SUCCESS;
}

} // namespace CougSat1
} // namespace GUI