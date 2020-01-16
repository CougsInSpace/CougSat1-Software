#include "Radio.h"

#include <chrono>
#include <spdlog/spdlog.h>

namespace Radio {

std::atomic<bool>            Radio::running  = false;
std::unique_ptr<std::thread> Radio::threadRX = nullptr;
std::unique_ptr<std::thread> Radio::threadTX = nullptr;

std::list<std::unique_ptr<Communications::Frame> > Radio::frames;

std::mutex Radio::mutex;

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
  Communications::Frame frame;
  while (running) {
    // frame.put(symbolSource.getBytes());
    // if(frame.isComplete())
    //   frame.process();
    spdlog::info("Loop RX");
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

/**
 * @brief TX thread, sends frames when the queue is not empty
 *
 */
void Radio::loopTX() {
  while (running) {
    if (frames.empty())
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    else {
      spdlog::info("TODO TX frame");
      // symbolSink.put(frame.getBytes())
    }
  }
}

/**
 * @brief Generate and send a test telemetry to test CougSatNet
 *
 */
void Radio::sendTestTelemtry() {
  // Create telemetry.cst with values using perlin noise
  // Radio.add(new session(Telemetry.cst))
}

} // namespace Radio