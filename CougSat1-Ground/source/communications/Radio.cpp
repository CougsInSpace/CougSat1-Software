#include "Radio.h"

#include "components/communications/Frame.h"

#include <chrono>
#include <spdlog/spdlog.h>

namespace Communications {

std::atomic<bool>            Radio::running  = false;
std::unique_ptr<std::thread> Radio::threadRX = nullptr;
std::unique_ptr<std::thread> Radio::threadTX = nullptr;

std::unique_ptr<Session>                    Radio::currentSession;
std::unique_ptr<SymbolSink::SymbolSink>     Radio::symbolSink;
std::unique_ptr<SymbolSource::SymbolSource> Radio::symbolSource;

std::mutex Radio::mutex;

int16_t Radio::currentSessionID = -1;

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
  if (threadRX && threadRX->joinable())
    threadRX->join();
  if (threadTX && threadTX->joinable())
    threadTX->join();
}

/**
 * @brief RX thread, listens to the IQ source for frames
 * Once a frame is successfully received, it is processed
 *
 */
void Radio::loopRX() {
  std::unique_ptr<Frame> frame = std::make_unique<Frame>();
  while (running) {
    if (symbolSource == nullptr) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      continue;
    }

    try {
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
    }
  }
}

/**
 * @brief TX thread, sends frames when the queue is not empty
 *
 */
void Radio::loopTX() {
  while (running) {
    if (currentSession == nullptr || currentSession->isTXEmpty())
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    else if (symbolSink == nullptr)
      throw std::exception("No symbol sink for TX");
    else {
      std::unique_ptr<Frame> frame = currentSession->getNextFrame();
      while (!frame->isDone())
        symbolSink->add(frame->getNextTXByte());
    }
  }
}

} // namespace Communications