#include "QPSK.h"

namespace Communications {
namespace SymbolSource {

/**
 * @brief Construct a new QPSK::QPSK object
 *
 * @param symbolFrequency
 */
QPSK::QPSK(const uint32_t symbolFrequency) : SymbolSource(symbolFrequency) {}

/**
 * @brief Destroy the QPSK::QPSK object
 *
 */
QPSK::~QPSK() {}

/**
 * @brief Get a byte from the source
 * All 8b will be used, if a symbol is 2b, byte will contain 4 symbols
 *
 * @param byte buffer
 */
uint8_t QPSK::getByte() {
  // TODO this algorithm is fake
  int16_t i = 0, q = 0;
  int32_t averageI = 0, averageQ = 0;
  for (uint8_t count = 0; count < 5; ++count) {
    iqSource->getIQ(i, q);
    averageI += i;
    averageQ += q;
  }
  averageI /= 5;
  averageQ /= 5;
  if (iqCallback != nullptr)
    iqCallback(averageI, averageQ);

  return i;
}

} // namespace SymbolSource
} // namespace Communications
