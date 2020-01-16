#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SINK_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SINK_H_

#include "../IQSink/IQSink.h"

#include <memory>
#include <stdint.h>

namespace Communications {
namespace SymbolSink {

class SymbolSink {
public:
  /**
   * @brief Construct a new SymbolSink object
   *
   */
  SymbolSink() {}

  /**
   * @brief Destroy the SymbolSink object
   *
   */
  virtual ~SymbolSink() {}

  /**
   * @brief Set the iq sink to use to modulate symbols
   * Takes ownership of source
   *
   * @param sink
   */
  void setIQSink(std::unique_ptr<IQSink::IQSink> sink) {
    iqSink = std::move(sink);
  }

  /**
   * @brief Add a byte to the sink
   * All 8b will be used, if a symbol is 2b, byte will contain 4 symbols
   *
   * @param byte buffer
   */
  virtual inline void add(uint8_t byte) = 0;

protected:
  std::unique_ptr<IQSink::IQSink> iqSink = nullptr;
};

} // namespace SymbolSink
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SINK_H_ */