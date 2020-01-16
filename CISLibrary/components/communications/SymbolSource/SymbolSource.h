#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_H_

#include "../IQSource/IQSource.h"

#include <memory>
#include <stdint.h>

namespace Communications {
namespace SymbolSource {

class SymbolSource {
public:
  /**
   * @brief Construct a new SymbolSource object
   *
   */
  SymbolSource() {}

  /**
   * @brief Destroy the SymbolSource object
   *
   */
  virtual ~SymbolSource() {}

  /**
   * @brief Set the iq source to use to demodulate symbols
   * Takes ownership of source
   *
   * @param source
   */
  void setIQSource(std::unique_ptr<IQSource::IQSource> source) {
    iqSource = std::move(source);
  }

  /**
   * @brief Get a byte from the source
   * All 8b will be used, if a symbol is 2b, byte will contain 4 symbols
   *
   * @param byte buffer
   * @throw std::underflow_error if there is no signal
   */
  virtual inline uint8_t getByte() = 0;

protected:
  std::unique_ptr<IQSource::IQSource> iqSource = nullptr;
};

} // namespace SymbolSource
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_H_ */