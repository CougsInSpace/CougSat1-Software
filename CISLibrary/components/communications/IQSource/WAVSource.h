#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_WAV_SOURCE_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_WAV_SOURCE_H_

#include "IQSource.h"

#include <AudioFile.h>
#include <ResultCode.h>
#include <string>

namespace Communications {

class WAVSource : public IQSource {
public:
  WAVSource(const WAVSource &) = delete;
  WAVSource & operator=(const WAVSource &) = delete;

  WAVSource(const char * filename);
  ~WAVSource();

  ResultCode_t init();

  ResultCode_t getIQ(int16_t & dataI, int16_t & dataQ);

private:
  std::string filename;

  AudioFile<double> file;
  size_t            fileIndex = 0;
};

} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_WAV_SOURCE_H_ */