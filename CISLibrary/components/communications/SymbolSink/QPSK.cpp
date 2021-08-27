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
  for (int8_t i = 6; i >= 0; i -= 2) {
    uint8_t symbol = (byte >> i) & 0x03;
    // Differential QPSK, the symbol rotates from the current
    currentPhase = (currentPhase + symbol) & 0x03;

    // Repeat the sample to get the correct samples per symbol
    for (uint32_t sampleCount = 0; sampleCount < samplesPerSymbol;
         ++sampleCount) {
      iqSink->addIQ((currentPhase & 0x02) ? 0x7FFF : -0x8000,
          (currentPhase & 0x01) ? 0x7FFF : -0x8000);
    }
  }
}

} // namespace SymbolSink
} // namespace Communications