#include "RTLSDR.h"

#ifdef WIN32

namespace Communications {
namespace IQSource {

/**
 * @brief Construct a new RTLSDR::RTLSDR object
 *
 */
RTLSDR::RTLSDR(const uint32_t centerFreq) :
  centerFrequency(centerFreq), IQSource(1024000) {
  int deviceCount = rtlsdr_get_device_count();
  if (deviceCount == 0)
    throw std::exception("No RTL SDR device detected");

  int error;
  for (int i = 0; i < deviceCount; i++) {
    error = rtlsdr_open(&device, i);
    if (error == 0)
      break;
  }

  error = rtlsdr_set_sample_rate(device, sampleFrequency);
  if (error)
    throw std::exception("Failed to set sample rate");

  error = rtlsdr_set_center_freq(device, centerFrequency);
  if (error)
    throw std::exception("Failed to set center frequency");

  // Automatic gain control on
  error = rtlsdr_set_tuner_gain_mode(device, 0);
  if (error)
    throw std::exception("Failed to set AGC on");

  error = rtlsdr_reset_buffer(device);
  if (error)
    throw std::exception("Failed to reset buffer");

  running = true;
  thread  = std::thread(&RTLSDR::loop, this);
}

/**
 * @brief Destroy the RTLSDR::RTLSDR object
 *
 */
RTLSDR::~RTLSDR() {
  stop();
  rtlsdr_close(device);
}

/**
 * @brief Asynchronous callback for reading the RTL-SDR
 *
 * @param buf buffer, interleaved
 * @param len length of buffer
 * @param context
 */
void RTLSDR::asyncCallback(uint8_t * buf, uint32_t len, void * context) {
  RTLSDR * thisCTX = (RTLSDR *)context;
  if (context) {
    while (len > 1) {
      if (!thisCTX->running) {
        rtlsdr_cancel_async(thisCTX->device);
        return;
      }
      PairInt16_t pair = {static_cast<int16_t>(*buf) << 8,
          static_cast<int16_t>(*(buf + 1)) << 8};
      while (thisCTX->running && !thisCTX->iqBuffer.push(pair))
        ; // Try again

      buf += 2;
      len -= 2;
    }
  }
}

/**
 * @brief Polls the RTL SDR dongle
 *
 */
void RTLSDR::loop() {
  // Start the async polling, blocks until cancel is called
  int result = rtlsdr_read_async(device, asyncCallback, this, 0, 0);
  if (result) {
    printf("Error: %d\n", result);
  }
}

/**
 * @brief Stop the RTL-SDR reading thread
 *
 */
void RTLSDR::stop() {
  running    = false;
  int result = rtlsdr_cancel_async(device);
  if (result)
    throw std::exception("Failed to cancel RTL SDR async");
  thread.join();
}

} // namespace IQSource
} // namespace Communications

#endif /* WIN32 */
