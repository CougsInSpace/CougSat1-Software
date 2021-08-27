#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_WAV_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_WAV_H_

#include "IQSource.h"

#include <cstring>
#include <stdio.h>

namespace Communications {
namespace IQSource {

class WAV : public IQSource {
public:
  WAV(const WAV &) = delete;
  WAV & operator=(const WAV &) = delete;

  WAV(FILE * file);
  ~WAV();

  void getIQ(int16_t & dataI, int16_t & dataQ);

private:
  enum class Format_t : uint16_t {
    PCM        = 0x0001,
    IEEE_FLOAT = 0x0003,
    A_LAW      = 0x0006,
    MU_LAW     = 0x0007,
    EXTENSIBLE = 0xFFFE
  };

  template <typename T> void read(T & number, const uint8_t bytes);

  FILE * file = nullptr;

  uint16_t channels;
  uint16_t bitsPerSample;
};

} // namespace IQSource
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_WAV_H_ */