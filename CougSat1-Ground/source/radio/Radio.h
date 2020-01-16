#ifndef _RADIO_H_
#define _RADIO_H_

#include "components/communications/Frame.h"

#include <atomic>
#include <list>
#include <memory>
#include <mutex>
#include <thread>

namespace Radio {

class Radio {
public:
  Radio() = delete;

  static void start();
  static void stop();

private:
  static void loopRX();
  static void loopTX();

  static void sendTestTelemtry();

  static std::atomic<bool> running;

  static std::unique_ptr<std::thread> threadRX;
  static std::unique_ptr<std::thread> threadTX;

  static std::list<std::unique_ptr<Communications::Frame> > frames;

  static std::mutex mutex;
};

} // namespace Radio

#endif /* _RADIO_H_ */