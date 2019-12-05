#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_RTL_SDR_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_RTL_SDR_H_

#include "IQSource.h"

#include <ResultCode.h>
#include <rtl-sdr.h>
#include <thread>

namespace Communications {
namespace IQSource {

class RTLSDR : public IQSource {
public:
  RTLSDR(const RTLSDR &) = delete;
  RTLSDR & operator=(const RTLSDR &) = delete;

  RTLSDR(const uint32_t centerFreq);
  ~RTLSDR();

  ResultCode_t init();

private:
  void loop();

  ResultCode_t stop();

  static void asyncCallback(uint8_t * buf, uint32_t len, void * context);

  std::thread   thread;
  volatile bool running = false;

  uint32_t       centerFrequency;
  rtlsdr_dev_t * device;

  const uint32_t SAMPLE_FREQ = 1024000;
};

} // namespace IQSource
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_RTL_SDR_H_ */