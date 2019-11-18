#include <Ehbanana.h>

#include <spdlog/spdlog.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#include <Windows.h>
#include <chrono>
#include <thread>

#include "gui/GUI.h"
#include "radio/RadioRX.h"

/**
 * @brief Logger callback
 * Prints the message string to the destination stream, default: stdout
 *
 * @param EBLogLevel_t log level
 * @param char * string
 */
void __stdcall logEhbanana(const EBLogLevel_t level, const char * string) {
  switch (level) {
    case EBLogLevel_t::EB_DEBUG:
      // spdlog::debug(string);
      break;
    case EBLogLevel_t::EB_INFO:
      // spdlog::info(string);
      break;
    case EBLogLevel_t::EB_WARNING:
      spdlog::warn(string);
      break;
    case EBLogLevel_t::EB_ERROR:
      spdlog::error(string);
      break;
    case EBLogLevel_t::EB_CRITICAL:
      spdlog::critical(string);
      break;
  }
}

/**
 * @brief Configure logging
 *
 * @param fileName of the log file, nullptr for no logging to file
 * @param rotatingLogs will rotate between 3 files and overwrite the oldest if
 * true, or overwrite the single file if false
 * @param showConsole will open a console output window if true
 * @return Result
 */
Result configureLogging(
    const char * fileName, bool rotatingLogs, bool showConsole) {
  std::vector<spdlog::sink_ptr> sinks;

  if (showConsole) {
    // Create a console and remove its close button
    if (AllocConsole()) {
      HWND hwnd = GetConsoleWindow();
      if (hwnd != NULL) {
        HMENU hMenu = GetSystemMenu(hwnd, FALSE);
        if (hMenu != NULL)
          DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
      }
      freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
    } else {
      MessageBoxA(NULL, "Log console initialization failed", "Error", MB_OK);
      std::cout << "Failed to AllocConsole with Win32 error: " << GetLastError()
                << std::endl;
      return ResultCode_t::OPEN_FAILED + "AllocConsole";
    }
    sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
  }

  if (fileName != nullptr) {
    try {
      if (rotatingLogs)
        sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            fileName, 5 * 1024 * 1024, 3));
      else
        sinks.push_back(
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(fileName));
    } catch (const spdlog::spdlog_ex & e) {
      MessageBoxA(NULL, "Log initialization failed", "Error", MB_OK);
      std::cout << "Log initialization failed: " << e.what() << std::endl;
      return ResultCode_t::OPEN_FAILED +
             ("Opening log files to " + std::string(fileName));
    }
  }

  std::shared_ptr<spdlog::logger> logger =
      std::make_shared<spdlog::logger>("", begin(sinks), end(sinks));
  spdlog::set_default_logger(logger);

#ifdef DEBUG
  spdlog::set_level(spdlog::level::debug);
#endif

  return ResultCode_t::SUCCESS;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  Result result = configureLogging("log.log", true, true);

  spdlog::info("Cougs in Space Ground starting");

  EBSetLogger(logEhbanana);

  result = GUI::GUI::Instance()->init();
  if (!result) {
    spdlog::error((result + "Initializing GUI").getMessage());
    return static_cast<int>(result.getCode());
  }

  Radio::RadioRX radioRX;
  result = radioRX.init(GUI::GUI::Instance()->getConstellationBuffer());
  if (!result) {
    spdlog::error((result + "Initializing RadioRX").getMessage());
    return static_cast<int>(result.getCode());
  }
  radioRX.start();

  result = GUI::GUI::Instance()->run();
  if (!result) {
    spdlog::error((result + "Running GUI").getMessage());
    return static_cast<int>(result.getCode());
  }

  result = GUI::GUI::Instance()->deinit();
  if (!result) {
    spdlog::error((result + "Deiniting GUI").getMessage());
    return static_cast<int>(result.getCode());
  }

  radioRX.stop();

  spdlog::info("Cougs in Space Ground complete");
  spdlog::default_logger()->flush();
  return static_cast<int>(ResultCode_t::SUCCESS);
}