#include "WAVSource.h"

namespace Communications {

/**
 * @brief Construct a new WAVSource::WAVSource object
 *
 * @param filename
 */
WAVSource::WAVSource(const char * filename) : filename(filename) {}

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
  if (!file.load(filename))
    return ResultCode_t::OPEN_FAILED;

  if (file.getNumChannels() != 2) {
    // TODO assume single channel files are audio
    return ResultCode_t::INVALID_DATA;
  }

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
  if (fileIndex > file.getNumSamplesPerChannel())
    return ResultCode_t::END_OF_FILE; // End of file
  dataI = static_cast<int16_t>(file.samples[0][fileIndex] * 32768.0);
  dataQ = static_cast<int16_t>(file.samples[1][fileIndex] * 32768.0);
  ++fileIndex;
  return ResultCode_t::SUCCESS;
}

} // namespace Communications
