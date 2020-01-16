#ifndef _COMMUNICATIONS_RADIO_H_
#define _COMMUNICATIONS_RADIO_H_

#include "components/communications/Session.h"
#include "components/communications/SymbolSink/SymbolSink.h"
#include "components/communications/SymbolSource/SymbolSource.h"

#include <atomic>
#include <list>
#include <memory>
#include <mutex>
#include <thread>

namespace Communications {

class Radio {
public:
  Radio() = delete;

  static void start();
  static void stop();

private:
  static void loopRX();
  static void loopTX();

  static std::atomic<bool> running;

  static std::unique_ptr<std::thread> threadRX;
  static std::unique_ptr<std::thread> threadTX;

  static std::unique_ptr<Session>                    currentSession;
  static std::unique_ptr<SymbolSink::SymbolSink>     symbolSink;
  static std::unique_ptr<SymbolSource::SymbolSource> symbolSource;

  static std::mutex mutex;

  static int16_t currentSessionID;
};

} // namespace Communications

#endif /* _COMMUNICATIONS_RADIO_H_ */