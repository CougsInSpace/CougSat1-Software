#include "GUI.h"

#include "Radio.h"
#include "Root.h"
#include "cougsat-1/ADCS.h"
#include "cougsat-1/CDH.h"
#include "cougsat-1/Comms.h"
#include "cougsat-1/CougSat1.h"
#include "cougsat-1/ECS.h"
#include "cougsat-1/EPS.h"
#include "cougsat-1/IFJR.h"
#include "cougsat-1/PayloadCamera.h"
#include "cougsat-1/PayloadPlant.h"

#include <spdlog/spdlog.h>

namespace GUI {

/**
 * @brief Initialize and open the GUI
 *
 */
void GUI::init() {
  EBGUISettings_t guiSettings;
  guiSettings.configRoot = "config";
  guiSettings.httpRoot   = "http";

  EBError_t error = EBCreate(guiSettings);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback("/", Root::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback("/radio/", Radio::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback("/cougsat-1/", CougSat1::CougSat1::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback("/cougsat-1/adcs/", CougSat1::ADCS::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback("/cougsat-1/c&dh/", CougSat1::CDH::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback("/cougsat-1/comms/", CougSat1::Comms::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback("/cougsat-1/ecs/", CougSat1::ECS::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback("/cougsat-1/eps/", CougSat1::EPS::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback("/cougsat-1/ifjr/", CougSat1::IFJR::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback(
      "/cougsat-1/payload-camera/", CougSat1::PayloadCamera::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBAttachCallback(
      "/cougsat-1/payload-plant/", CougSat1::PayloadPlant::callback);
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));

  error = EBLaunch();
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));
}

/**
 * @brief Run the GUI, blocks until the GUI is closed
 *
 */
void GUI::run() {
  // Block until the GUI is done
  while (!EBIsDone()) {
    Radio::sendConstellationDiagram();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}

/**
 * @brief Deinitialize the GUI
 *
 */
void GUI::deinit() {
  EBError_t error = EBDestroy();
  if (EB_FAILED(error))
    throw std::exception(EBErrorName(error));
}

} // namespace GUI