#ifndef _RADIO_RADIO_RX_H_
#define _RADIO_RADIO_RX_H_

#include <components/communications/IQSource/IQSource.h>
#include <tools/CircularBuffer.h>

#include <FruitBowl.h>
#include <atomic>
#include <thread>

namespace Radio {

class RadioRX {
public:
  RadioRX(const RadioRX &) = delete;
  RadioRX & operator=(const RadioRX &) = delete;

  /**
   * @brief Get the singleton instance
   *
   * @return RadioRX*
   */
  static RadioRX * Instance() {
    static RadioRX instance;
    return &instance;
  }

  ~RadioRX();

  Result init(CircularBuffer<PairDouble_t> * guiBuf);

  Result setIQFile(FILE * file);

  Result setRTLSDR(const uint32_t centerFreq = 104300000);

  void start();
  void stop();

private:
  /**
   * @brief Construct a new Radio R X object
   *
   */
  RadioRX() {}

  void run();

  std::thread *     thread  = nullptr;
  std::atomic<bool> running = false;

  Communications::IQSource::IQSource * iqSource = nullptr;

  CircularBuffer<PairDouble_t> * gui = nullptr;
};

}; // namespace Radio

#endif /* _RADIO_RADIO_RX_H_ */