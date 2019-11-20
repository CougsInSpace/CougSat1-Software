#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_RTL_SDR_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_RTL_SDR_H_

#include "IQSource.h"

#include <ResultCode.h>

namespace Communications {
namespace IQSource {

class RTLSDR : public IQSource {
public:
  RTLSDR(const RTLSDR &) = delete;
  RTLSDR & operator=(const RTLSDR &) = delete;

  RTLSDR();
  ~RTLSDR();

  ResultCode_t init();

private:
};

} // namespace IQSource
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_RTL_SDR_H_ */