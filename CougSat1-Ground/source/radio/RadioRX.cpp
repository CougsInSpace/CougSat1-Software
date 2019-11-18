#include "RadioRX.h"

#include <chrono>
#include <spdlog/spdlog.h>

#include <components/communications/IQSource/WAVSource.h>

namespace Radio {

/**
 * @brief Construct a new RadioRX:: RadioRX object
 *
 */
RadioRX::RadioRX() {}

/**
 * @brief Destroy the RadioRX:: RadioRX object
 *
 */
RadioRX::~RadioRX() {
  stop();
  delete iqSource;
}

/**
 * @brief Initialize the Radio RX, create the IQSource
 *
 * @param guiBuf to plot to
 * @return Result
 */
Result RadioRX::init(CircularBuffer<PairDouble_t> * guiBuf) {
  gui = guiBuf;

  iqSource = new Communications::WAVSource(
      "test/IQExamples/QPSK - MATLAB - 13kHz offset.wav");
  ResultCode_t result = iqSource->init();
  if (!result) {
    delete iqSource;
    iqSource = new Communications::WAVSource(
        "../test/IQExamples/QPSK - MATLAB - 13kHz offset.wav");
    result = iqSource->init();
    if (!result)
      return result + ("Failed to initialize WAVSource");
  }
  return ResultCode_t::SUCCESS;
}

/**
 * @brief Start the RadioRX loop
 *
 */
void RadioRX::start() {
  stop();
  running = true;
  thread  = new std::thread(&RadioRX::run, this);
}

/**
 * @brief Stop the RadioRX loop
 *
 */
void RadioRX::stop() {
  running = false;
  if (thread == nullptr)
    return;
  if (thread->joinable())
    thread->join();
  delete thread;
  thread = nullptr;
}

/**
 * @brief Run the RadioRX loop, listen for signals
 *
 */
void RadioRX::run() {
  int16_t      i;
  int16_t      q;
  ResultCode_t result;
  while (running) {
    result = iqSource->getIQ(i, q);
    if (!result) {
      spdlog::error((result + "Failed to get IQ").getMessage());
      running = false;
      return;
    }
    while (running && !gui->push({i / 32768.0, q / 32768.0}))
      std::this_thread::sleep_for(millis_t(10));
    std::this_thread::sleep_for(millis_t(1));
  }
}

} // namespace Radio