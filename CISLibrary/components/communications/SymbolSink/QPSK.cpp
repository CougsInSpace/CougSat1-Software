#include "QPSK.h"

namespace Communications {
namespace SymbolSink {

/**
 * @brief Construct a new QPSK::QPSK object
 *
 */
QPSK::QPSK(const uint32_t symbolFrequency) : SymbolSink(symbolFrequency) {}

/**
 * @brief Destroy the QPSK::QPSK object
 *
 */
QPSK::~QPSK() {}

/**
 * @brief Add a byte to the sink
 * All 8b will be used, if a symbol is 2b, byte will contain 4 symbols
 *
 * @param byte buffer
 */
void QPSK::add(uint8_t byte) {
  if (iqSink == nullptr)
    throw std::exception("QPSK iqSink is nullptr");

  // Calculate next IQ for the four symbols, add to IQ sink
  for (int i = 0; i < 10; ++i)
    iqSink->addIQ((byte & 0xF0) << 8, (byte & 0x0F) << 12);
}

} // namespace SymbolSink
} // namespace Communications