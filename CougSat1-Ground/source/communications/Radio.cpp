#include "Radio.h"

#include "components/communications/Frame.h"
#include "components/communications/SymbolSink/QPSK.h"
#include "components/communications/SymbolSource/QPSK.h"

#include <chrono>
#include <spdlog/spdlog.h>

namespace Communications {

std::atomic<bool>            Radio::running  = false;
std::unique_ptr<std::thread> Radio::threadRX = nullptr;
std::unique_ptr<std::thread> Radio::threadTX = nullptr;

std::unique_ptr<Session>                    Radio::currentSession = nullptr;
std::unique_ptr<SymbolSink::SymbolSink>     Radio::symbolSink     = nullptr;
std::unique_ptr<SymbolSource::SymbolSource> Radio::symbolSource   = nullptr;
std::unique_ptr<IQSink::IQSink>             Radio::iqSink         = nullptr;
std::unique_ptr<IQSource::IQSource>         Radio::iqSource       = nullptr;
std::unique_ptr<TestSatellite>              Radio::testSatellite  = nullptr;

std::mutex Radio::mutexRX;
std::mutex Radio::mutexTX;

int16_t Radio::currentSessionID = -1;

SymbolSource::ConstellationCallback_t Radio::constellationCallback = nullptr;

/**
 * @brief Start the radio threads
 *
 */
void Radio::start() {
  stop();

  running  = true;
  threadRX = std::make_unique<std::thread>(loopRX);
  threadTX = std::make_unique<std::thread>(loopTX);
}

/**
 * @brief Stop the radio threads
 *
 */
void Radio::stop() {
  running = false;
  if (testSatellite)
    testSatellite->stop();

  if (threadTX && threadTX->joinable())
    threadTX->join();
  if (threadRX && threadRX->joinable())
    threadRX->join();

  if (testSatellite) {
    // Sink and source test satellite, not unique
    // Release both so destructing happens only once
    iqSource.release();
    iqSink.release();
  }
}

/**
 * @brief Set the test mode, transmits telemetry every second with varying
 * amounts of distortion
 *
 * @param distortionFactor
 */
void Radio::setTestMode(uint8_t distortionFactor) {
  const std::lock_guard<std::mutex> lockRX(mutexRX);
  const std::lock_guard<std::mutex> lockTX(mutexTX);
  spdlog::debug("Setting test mode with {} distortion", distortionFactor);

  testSatellite = std::make_unique<TestSatellite>();
  testSatellite->setDistortion(distortionFactor);

  uint32_t symbolFrequency = 10000;

  symbolSink = std::make_unique<SymbolSink::QPSK>(symbolFrequency);
  iqSink     = std::unique_ptr<IQSink::IQSink>(testSatellite.get());
  symbolSink->setIQSink(testSatellite.get());

  symbolSource = std::make_unique<SymbolSource::QPSK>(symbolFrequency);
  iqSource     = std::unique_ptr<IQSource::IQSource>(testSatellite.get());
  symbolSource->setIQSource(testSatellite.get());

  if (constellationCallback != nullptr)
    symbolSource->setConstellationCallback(constellationCallback);
}

/**
 * @brief RX thread, listens to the IQ source for frames
 * Once a frame is successfully received, it is processed
 *
 */
void Radio::loopRX() {
  try {
    std::unique_ptr<Frame> frame = std::make_unique<Frame>();
    while (running) {
      try {
        const std::lock_guard<std::mutex> lock(mutexRX);
        if (symbolSource == nullptr) {
          std::this_thread::sleep_for(std::chrono::milliseconds(1));
          continue;
        }

        frame->add(symbolSource->getByte());

        if (frame->isDone()) {
          if (currentSession == nullptr) {
            currentSession = std::make_unique<Session>();
            currentSession->add(std::move(frame));
            currentSessionID = currentSession->getID();
          } else
            currentSession->add(std::move(frame));

          frame = std::make_unique<Frame>();
        }
      } catch (const std::underflow_error & /*e*/) {
        spdlog::info("No RX signal");
      } // Other exceptions let throw
    }
  } catch (const std::exception & e) {
    spdlog::error("Exception encountered in loopRX: {}", e.what());
    running = false;
  }
}

/**
 * @brief TX thread, sends frames when the queue is not empty
 *
 */
void Radio::loopTX() {
  try {
    while (running) {
      const std::lock_guard<std::mutex> lock(mutexTX);
      // if (currentSession == nullptr || currentSession->isTXEmpty()) {
      //   std::this_thread::sleep_for(std::chrono::milliseconds(1));
      //   continue;
      // }

      if (symbolSink == nullptr)
        // throw std::exception("No symbol sink for TX");
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      else {
        // std::unique_ptr<Frame> frame = currentSession->getNextFrame();
        // while (!frame->isDone())
        //   symbolSink->add(frame->getNextTXByte());
        symbolSink->add(static_cast<uint8_t>(rand()));
      }
    }
  } catch (const std::exception & e) {
    spdlog::error("Exception encountered in loopTX: {}", e.what());
    running = false;
  }
}

/**
 * @brief Enqueue a file for transmit
 *
 */
void Radio::enqueueTX() {
  // TODO enqueue real files
}

/**
 * @brief Set the constellation callback to update a constellation diagram
 *
 * @param callback
 */
void Radio::setConstellationCallback(
    SymbolSource::ConstellationCallback_t callback) {
  const std::lock_guard<std::mutex> lockRX(mutexRX);
  constellationCallback = callback;
  if (symbolSource)
    symbolSource->setConstellationCallback(callback);
}

} // namespace Communications