#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SINK_QPSK_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SINK_QPSK_H_

#include "SymbolSink.h"

namespace Communications {
namespace SymbolSink {

class QPSK : public SymbolSink {
public:
  QPSK(const uint32_t symbolFrequency);
  ~QPSK();

  void add(uint8_t byte);

private:
  uint8_t currentPhase = 0;
};

} // namespace SymbolSink
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SINK_QPSK_H_ */