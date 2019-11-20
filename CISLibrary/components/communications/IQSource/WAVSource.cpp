#include "WAVSource.h"

namespace Communications {

/**
 * @brief Construct a new WAVSource::WAVSource object
 *
 * Currently only compatible with PCM data
 * TODO implement other wave data formats
 *
 * @param file to read, should point to start of file
 */
WAVSource::WAVSource(FILE * file) : file(file) {}

/**
 * @brief Destroy the WAVSource::WAVSource object
 *
 */
WAVSource::~WAVSource() {}

/**
 * @brief Init the WAVSource by loading the audio file
 *
 * @return ResultCode_t
 */
ResultCode_t WAVSource::init() {
  ResultCode_t result = ResultCode_t::SUCCESS;

  // Header chunk
  char buf[5];
  if (fgets(buf, 5, file) == nullptr)
    return ResultCode_t::END_OF_FILE;
  if (strncmp(buf, "RIFF", 4) != 0)
    return ResultCode_t::INVALID_DATA;

  // Skip chunk size
  uint32_t bufInt;
  result = read(bufInt, 4);
  if (!result)
    return result;

  if (fgets(buf, 5, file) == nullptr)
    return ResultCode_t::END_OF_FILE;
  if (strncmp(buf, "WAVE", 4) != 0)
    return ResultCode_t::INVALID_DATA;

  // Format chunk
  if (fgets(buf, 5, file) == nullptr)
    return ResultCode_t::END_OF_FILE;
  if (strncmp(buf, "fmt ", 4) != 0)
    return ResultCode_t::INVALID_DATA;

  result = read(bufInt, 4);
  if (!result)
    return result;
  // 16B for PCM data
  if (bufInt != 16)
    return ResultCode_t::INVALID_DATA;

  result = read(bufInt, 2);
  if (!result)
    return result;
  if (bufInt != static_cast<uint16_t>(Format_t::PCM))
    return ResultCode_t::INVALID_DATA;

  result = read(channels, 2);
  if (!result)
    return result;
  // TODO only 2 channel IQ is supported, add 1 channel audio
  if (channels != 2)
    return ResultCode_t::INVALID_DATA;

  result = read(sampleFreq, 4);
  if (!result)
    return result;

  // Skip bytes per second
  result = read(bufInt, 4);
  if (!result)
    return result;

  // Skip block align
  result = read(bufInt, 2);
  if (!result)
    return result;

  result = read(bitsPerSample, 2);
  if (!result)
    return result;
  // TODO support other bit depths
  if (bitsPerSample != 16)
    return ResultCode_t::INVALID_DATA;

  // Data chunk
  if (fgets(buf, 5, file) == nullptr)
    return ResultCode_t::END_OF_FILE;
  if (strncmp(buf, "data", 4) != 0)
    return ResultCode_t::INVALID_DATA;

  // Skip chunk size
  result = read(bufInt, 4);
  if (!result)
    return result;

  // File is now at the data region
  // Data is interleaved
  // ch0, ch1, ch0, ch1, ch0, ch1, ...
  // ch0, ch1, ch2, ch0, ch1, ch2, ...

  return ResultCode_t::SUCCESS;
}

/**
 * @brief Get the IQ data from the source
 *
 * @param dataI buffer
 * @param dataQ buffer
 * @return ResultCode_t
 */
ResultCode_t WAVSource::getIQ(int16_t & dataI, int16_t & dataQ) {
  if (iqBuffer.size() == 0) {
    ResultCode_t result = ResultCode_t::SUCCESS;
    int32_t      buf;
    while (result == ResultCode_t::SUCCESS && iqBuffer.size() < 255) {
      PairInt16_t pair;
      result = read(pair.a, 2);
      if (!result)
        return result;

      result = read(pair.b, 2);
      if (!result)
        return result;

      result = iqBuffer.push(pair);
    }
  }
  return IQSource::getIQ(dataI, dataQ);
}

/**
 * @brief Read an number from the file with the number of bytes (up to eight)
 *
 * @param number to read into
 * @param bytes count to read
 * @return ResultCode_t
 */
template <typename T>
ResultCode_t WAVSource::read(T & number, const uint8_t bytes) {
  // WAV is little endian
  uint64_t temp = 0;
  int      c;
  for (uint8_t i = 0; i < bytes; ++i) {
    if ((c = fgetc(file)) == EOF)
      return ResultCode_t::END_OF_FILE;
    temp |= (c & 0xFF) << (8 * i);
  }
  number = static_cast<T>(temp);
  return ResultCode_t::SUCCESS;
}

} // namespace Communications
