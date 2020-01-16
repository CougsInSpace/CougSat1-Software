#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_H_

#include "../IQSource/IQSource.h"
#include <ResultCode.h>
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
  virtual ~SymbolSource() {
    delete iqSource;
  }

  /**
   * @brief Set the iq source to use to demodulate symbols
   * Takes ownership of source
   *
   * @param source
   */
  void setIQSource(IQSource::IQSource * source) {
    delete iqSource;
    iqSource = source;
  }

  /**
   * @brief Get a byte from the source
   * All 8b will be used, if a symbol is 2b, byte will contain 4 symbols
   *
   * @param byte buffer
   * @return ResultCode_t
   */
  virtual inline ResultCode_t getByte(uint8_t & byte) = 0;

protected:
  IQSource::IQSource * iqSource = nullptr;
};

} // namespace SymbolSource
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_H_ */