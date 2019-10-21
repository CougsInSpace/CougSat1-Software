#include "GUI.h"

#include <spdlog/spdlog.h>
#include <thread>

namespace GUI {

/**
 * @brief Destroy the GUI::GUI object
 *
 */
GUI::~GUI() {
  delete root;
}

/**
 * @brief Process incoming message from the GUI
 *
 * @param msg to process
 * @return ResultCode_t error code
 */
ResultCode_t __stdcall GUI::guiProcess(const EBMessage_t & msg) {
  switch (msg.type) {
    case EBMSGType_t::STARTUP:
      spdlog::info("Server starting up");
      break;
    case EBMSGType_t::SHUTDOWN:
      spdlog::info("Server shutting down");
      break;
    case EBMSGType_t::INPUT: {
      Result result = GUI::GUI::Instance()->handleInput(msg);
      if (result == ResultCode_t::UNKNOWN_HASH) {
        spdlog::warn(result.getMessage());
        return ResultCode_t::SUCCESS;
      }
      if (!result)
        spdlog::error(result.getMessage());
      return result.getCode();
    }
    default:
      return EBDefaultGUIProcess(msg);
  }
  return ResultCode_t::SUCCESS;
}

/**
 * @brief Initialize the GUI by creating an Ehbanana instance
 *
 * @return Result
 */
Result GUI::init() {
  EBGUISettings_t settings;
  settings.guiProcess = guiProcess;
  settings.configRoot = "config";
  settings.httpRoot   = "http";

  ResultCode_t resultCode = EBCreateGUI(settings, gui);
  if (!resultCode)
    return resultCode + "EBCreateGUI";

  root  = new Root(gui);
  radio = new Radio(gui);

  cougSat1 = new CougSat1(gui);

  resultCode = EBShowGUI(gui);
  if (!resultCode)
    return resultCode + "EBShowGUI";

  return ResultCode_t::SUCCESS;
}

/**
 * @brief Run a loop checking for messages from the GUI, will block until GUI is
 * complete
 *
 * @return Result
 */
Result GUI::run() {
  Result      result;
  EBMessage_t msg;
  while ((result = EBGetMessage(msg)) == ResultCode_t::INCOMPLETE ||
         result == ResultCode_t::NO_OPERATION) {
    // If no messages were processed, wait a bit to save CPU
    if (result == ResultCode_t::NO_OPERATION) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } else {
      result = EBDispatchMessage(msg);
      if (!result)
        return result + "EBDispatchMessage";
    }
  }
  return result + "EBGetMessage";
}

/**
 * @brief Deinitialize the GUI
 *
 * @return Result
 */
Result GUI::deinit() {
  Result result = EBDestroyGUI(gui);
  if (!result)
    return result + "EBDestroyGUI";
  return ResultCode_t::SUCCESS;
}

/**
 * @brief Handle the input from the GUI
 *
 * @param msg to handle
 * @return Result
 */
Result GUI::handleInput(const EBMessage_t & msg) {
  if (msg.id.getString().empty())
    return ResultCode_t::INVALID_DATA;
  spdlog::debug("{}|{}|{}", msg.href.getString(), msg.id.getString(),
      msg.value.getString());

  switch (msg.href.get()) {
    case Hash::calculateHash(""):
    case Hash::calculateHash("/"):
      return root->handleInput(msg);
    case Hash::calculateHash("/radio/"):
      return radio->handleInput(msg);
    case Hash::calculateHash("/cougsat-1/"):
    case Hash::calculateHash("/cougsat-1/adcs/"):
    case Hash::calculateHash("/cougsat-1/c&dh/"):
    case Hash::calculateHash("/cougsat-1/comms/"):
    case Hash::calculateHash("/cougsat-1/ecs/"):
    case Hash::calculateHash("/cougsat-1/eps/"):
    case Hash::calculateHash("/cougsat-1/ifjr/"):
    case Hash::calculateHash("/cougsat-1/payload-camera/"):
    case Hash::calculateHash("/cougsat-1/payload-plant/"):
      return cougSat1->handleInput(msg);
    default:
      return ResultCode_t::UNKNOWN_HASH +
             ("Input message's href: " + msg.href.getString());
  }
  return ResultCode_t::SUCCESS;
}

} // namespace GUI