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

  RadioRX();
  ~RadioRX();

  Result init(CircularBuffer<PairDouble_t> * guiBuf);

  void start();
  void stop();

private:
  void run();

  std::thread *     thread  = nullptr;
  std::atomic<bool> running = false;

  Communications::IQSource * iqSource = nullptr;

  CircularBuffer<PairDouble_t> * gui = nullptr;
};

}; // namespace Radio

#endif /* _RADIO_RADIO_RX_H_ */