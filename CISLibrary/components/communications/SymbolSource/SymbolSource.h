#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_H_

#include "../IQSource/IQSource.h"
#include "../tools/CircularBuffer.h"

#include <cstdint>

namespace Communications {
namespace SymbolSource {

/**
 * @brief Callback to update a constellation diagram
 *
 * @param i symbol in-phase component at sample point
 * @param q symbol quadrature component at sample point
 */
typedef void (*ConstellationCallback_t)(int16_t i, int16_t q);

class SymbolSource {
public:
  /**
   * @brief Construct a new SymbolSource object
   *
   * @param symbolFrequency
   */
  SymbolSource(const uint32_t symbolFrequency) :
    symbolFrequency(symbolFrequency) {}

  /**
   * @brief Destroy the SymbolSource object
   *
   */
  virtual ~SymbolSource() {}

  /**
   * @brief Set the iq source to use to demodulate symbols
   *
   * @param source
   */
  void setIQSource(IQSource::IQSource * source) {
    iqSource = source;
  }

  /**
   * @brief Get a byte from the source
   * All 8b will be used, if a symbol is 2b, byte will contain 4 symbols
   *
   * @param byte buffer
   * @throw std::underflow_error if there is no signal
   */
  virtual uint8_t getByte() = 0;

  /**
   * @brief Set the constellation callback to update a constellation diagram
   *
   * @param callback
   */
  void setConstellationCallback(ConstellationCallback_t callback) {
    iqCallback = callback;
  }

protected:
  IQSource::IQSource *    iqSource   = nullptr;
  ConstellationCallback_t iqCallback = nullptr;
  const uint32_t          symbolFrequency;
};

} // namespace SymbolSource
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_H_ */