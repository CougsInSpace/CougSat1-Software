#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SINK_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SINK_H_

#include "../IQSink/IQSink.h"

#include <cstdint>

namespace Communications {
namespace SymbolSink {

class SymbolSink {
public:
  /**
   * @brief Construct a new SymbolSink object
   *
   * @param symbolFrequency
   */
  SymbolSink(const uint32_t symbolFrequency) :
    symbolFrequency(symbolFrequency) {}

  /**
   * @brief Destroy the SymbolSink object
   *
   */
  virtual ~SymbolSink() {}

  /**
   * @brief Set the iq sink to use to modulate symbols
   *
   * @param sink
   */
  void setIQSink(IQSink::IQSink * sink) {
    iqSink           = sink;
    samplesPerSymbol = iqSink->getSampleFrequency() / symbolFrequency;
  }

  /**
   * @brief Add a byte to the sink
   * All 8b will be used, if a symbol is 2b, byte will contain 4 symbols
   *
   * @param byte buffer
   */
  virtual void add(uint8_t byte) = 0;

protected:
  IQSink::IQSink * iqSink           = nullptr;
  const uint32_t   symbolFrequency  = 0;
  uint32_t         samplesPerSymbol = 0;
};

} // namespace SymbolSink
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SINK_H_ */