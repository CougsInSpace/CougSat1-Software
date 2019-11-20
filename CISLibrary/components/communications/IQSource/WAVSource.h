#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_WAV_SOURCE_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_WAV_SOURCE_H_

#include "IQSource.h"

#include <ResultCode.h>
#include <cstring>
#include <stdio.h>

namespace Communications {

class WAVSource : public IQSource {
public:
  WAVSource(const WAVSource &) = delete;
  WAVSource & operator=(const WAVSource &) = delete;

  WAVSource(FILE * file);
  ~WAVSource();

  ResultCode_t init();

  ResultCode_t getIQ(int16_t & dataI, int16_t & dataQ);

private:
  enum class Format_t : uint16_t {
    PCM        = 0x0001,
    IEEE_FLOAT = 0x0003,
    A_LAW      = 0x0006,
    MU_LAW     = 0x0007,
    EXTENSIBLE = 0xFFFE
  };

  template <typename T> ResultCode_t read(T & number, const uint8_t bytes);

  FILE * file;

  uint16_t channels;
  uint32_t sampleFreq;
  uint16_t bitsPerSample;
};

} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_WAV_SOURCE_H_ */