#include <Ehbanana.h>

#include <spdlog/spdlog.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#include <Windows.h>

#include "gui/GUI.h"
#include "radio/Radio.h"

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
 */
void configureLogging(
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
      throw std::exception("Log console initialization failed");
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
      throw std::exception("Log file initialization failed");
    }
  }

  std::shared_ptr<spdlog::logger> logger =
      std::make_shared<spdlog::logger>("", begin(sinks), end(sinks));
  spdlog::set_default_logger(logger);

#ifdef DEBUG
  spdlog::set_level(spdlog::level::debug);
#endif
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  try {
    configureLogging("log.log", true, true);
    spdlog::info("Cougs in Space Ground starting");
    EBSetLogger(logEhbanana);

    GUI::GUI::init();
    Radio::Radio::start();

    GUI::GUI::run();

    Radio::Radio::stop();
    GUI::GUI::deinit();
  } catch (const std::exception & e) {
    spdlog::error(e.what());
    return -1;
  }

  spdlog::info("Cougs in Space Ground complete");
  spdlog::default_logger()->flush();
  return 0;
}