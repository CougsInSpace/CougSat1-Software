#include "RTLSDR.h"

#include <FruitBowl.h>

namespace Communications {
namespace IQSource {

/**
 * @brief Construct a new RTLSDR::RTLSDR object
 *
 */
RTLSDR::RTLSDR(const uint32_t centerFreq) : centerFrequency(centerFreq) {}

/**
 * @brief Destroy the RTLSDR::RTLSDR object
 *
 */
RTLSDR::~RTLSDR() {
  running = false;
  rtlsdr_close(device);
}

/**
 * @brief Init the RTLSDR by loading the USB device
 *
 * @return ResultCode_t
 */
ResultCode_t RTLSDR::init() {
  int deviceCount = rtlsdr_get_device_count();
  if (deviceCount == 0)
    return ResultCode_t::OPEN_FAILED;
  int result;
  for (int i = 0; i < deviceCount; i++) {
    result = rtlsdr_open(&device, i);
    if (result == 0)
      break;
  }

  result = rtlsdr_set_sample_rate(device, 2048000);
  if (result != 0)
    return ResultCode_t::WRITE_FAULT;

  result = rtlsdr_set_center_freq(device, centerFrequency);
  if (result != 0)
    return ResultCode_t::WRITE_FAULT;

  // Automatic gain control on
  result = rtlsdr_set_agc_mode(device, 1);
  if (result != 0)
    return ResultCode_t::WRITE_FAULT;

  running = true;
  thread  = std::thread(&RTLSDR::loop, this);

  return ResultCode_t::SUCCESS;
}

/**
 * @brief Polls the RTL SDR dongle
 *
 */
void RTLSDR::loop() {
  int result      = 0;
  int samplesRead = 0;

  // min: 1 * 512, max: 256 * 32 * 512
  uint32_t  outBlockSize = 16 * 32 * 512;
  uint8_t * buffer       = new uint8_t[outBlockSize];

  while (running) {
    result = rtlsdr_read_sync(device, buffer, outBlockSize, &samplesRead);
    if (result < 0) {
      fprintf(stderr, "WARNING: sync read failed.\n");
      running = false;
      break;
    }

    for (size_t i = 0; i < samplesRead; i += 2) {
      PairInt16_t pair = {static_cast<int16_t>(buffer[i]) << 8,
          static_cast<int16_t>(buffer[i + 1]) << 8};
      while (running && !iqBuffer.push(pair)) {
        std::this_thread::sleep_for(millis_t(1));
      }
    }

    if ((uint32_t)samplesRead < outBlockSize) {
      fprintf(stderr, "Short read, samples lost, exiting!\n");
      break;
    }
  }
  delete buffer;
}

} // namespace IQSource
} // namespace Communications
