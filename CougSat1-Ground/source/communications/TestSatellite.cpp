#include "TestSatellite.h"

#include "Radio.h"

namespace Communications {

/**
 * @brief Construct a new Test Satellite:: Test Satellite object
 *
 */
TestSatellite::TestSatellite() :
  thread(&TestSatellite::enqueueThread, this), IQSink(500000),
  IQSource(500000) {}

/**
 * @brief Destroy the Test Satellite:: Test Satellite object
 *
 */
TestSatellite::~TestSatellite() {
  stop();
}

/**
 * @brief Stop the test satellite
 *
 */
void TestSatellite::stop() {
  running = false;
  if (thread.joinable())
    thread.join();
}

/**
 * @brief Set the distortion amount applied to IQ loopback
 *
 * @param distortionFactor
 */
void TestSatellite::setDistortion(uint8_t distortionFactor) {
  distortion = distortionFactor;
}

/**
 * @brief Enqueue a random telemetry message every second
 *
 */
void TestSatellite::enqueueThread() {
  while (running) {
    // Radio::enqueue(TODO);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

/**
 * @brief Add IQ to the sink
 *
 * @param dataI buffer
 * @param dataQ buffer
 */
void TestSatellite::addIQ(int16_t dataI, int16_t dataQ) {
  // Apply distortion and add to IQSource queue
  // TODO variable distortion, phase noise, frequency offset, random jitter
  dataI = (dataI >> 1) + (rand() & 0x1000);
  dataQ = (dataQ >> 1) + (rand() & 0x1000);
  while (!iqBuffer.push({dataI, dataQ}) && running)
    // Block until element is pushed
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

/**
 * @brief Get the IQ data from the source
 *
 * @param dataI buffer
 * @param dataQ buffer
 */
void TestSatellite::getIQ(int16_t & dataI, int16_t & dataQ) {
  PairInt16_t iqPair;
  while (!iqBuffer.pop(iqPair) && running)
    // Block until element is pushed
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  dataI = iqPair.a;
  dataQ = iqPair.b;
}

} // namespace Communications