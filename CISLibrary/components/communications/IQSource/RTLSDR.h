#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_RTL_SDR_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_RTL_SDR_H_

#ifdef WIN32

#include "IQSource.h"

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

private:
  void loop();

  void stop();

  static void asyncCallback(uint8_t * buf, uint32_t len, void * context);

  std::thread   thread;
  volatile bool running = false;

  uint32_t       centerFrequency;
  rtlsdr_dev_t * device;
};

} // namespace IQSource
} // namespace Communications

#endif /* WIN32 */

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_RTL_SDR_H_ */