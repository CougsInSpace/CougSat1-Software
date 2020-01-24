#ifndef _COMMUNICATIONS_RADIO_H_
#define _COMMUNICATIONS_RADIO_H_

#include "TestSatellite.h"
#include "components/communications/IQSink/IQSink.h"
#include "components/communications/IQSource/IQSource.h"
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

  static void setTestMode(uint8_t distortionFactor = 0xFF);

  static void enqueueTX();

  static void setConstellationCallback(
      SymbolSource::ConstellationCallback_t callback);

private:
  static void loopRX();
  static void loopTX();

  static std::atomic<bool> running;

  static std::unique_ptr<std::thread> threadRX;
  static std::unique_ptr<std::thread> threadTX;

  static std::unique_ptr<Session>                    currentSession;
  static std::unique_ptr<SymbolSink::SymbolSink>     symbolSink;
  static std::unique_ptr<SymbolSource::SymbolSource> symbolSource;
  static std::unique_ptr<IQSink::IQSink>             iqSink;
  static std::unique_ptr<IQSource::IQSource>         iqSource;
  static std::unique_ptr<TestSatellite>              testSatellite;

  static std::mutex mutexRX;
  static std::mutex mutexTX;

  static int16_t currentSessionID;

  static SymbolSource::ConstellationCallback_t constellationCallback;
};

} // namespace Communications

#endif /* _COMMUNICATIONS_RADIO_H_ */