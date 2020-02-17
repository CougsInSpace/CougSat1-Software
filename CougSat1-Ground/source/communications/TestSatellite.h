#ifndef _COMMUNICATIONS_TEST_SATELLITE_H_
#define _COMMUNICATIONS_TEST_SATELLITE_H_

#include "components/communications/IQSink/IQSink.h"
#include "components/communications/IQSource/IQSource.h"

#include <atomic>
#include <thread>

namespace Communications {

class TestSatellite : public IQSink::IQSink, public IQSource::IQSource {
public:
  TestSatellite();
  ~TestSatellite();

  void stop();

  void setDistortion(uint8_t distortionFactor);

  void addIQ(int16_t dataI, int16_t dataQ);
  void getIQ(int16_t & dataI, int16_t & dataQ);

private:
  void enqueueThread();

  std::atomic<bool> running = true;
  std::thread       thread;

  uint8_t distortion = 0;
};

} // namespace Communications

#endif /* _COMMUNICATIONS_RADIO_H_ */