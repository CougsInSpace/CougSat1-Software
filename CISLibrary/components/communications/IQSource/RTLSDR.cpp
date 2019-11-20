#include "RTLSDR.h"

namespace Communications {
namespace IQSource {

/**
 * @brief Construct a new RTLSDR::RTLSDR object
 *
 */
RTLSDR::RTLSDR() {}

/**
 * @brief Destroy the RTLSDR::RTLSDR object
 *
 */
RTLSDR::~RTLSDR() {}

/**
 * @brief Init the RTLSDR by loading the USB device
 *
 * @return ResultCode_t
 */
ResultCode_t RTLSDR::init() {
  return ResultCode_t::NOT_SUPPORTED;
}

} // namespace IQSource
} // namespace Communications
