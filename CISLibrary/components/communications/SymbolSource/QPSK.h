#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_QPSK_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_QPSK_H_

#include "SymbolSource.h"

#include <cstdint>

namespace Communications {
namespace SymbolSource {

class QPSK : public SymbolSource {
public:
  QPSK(const uint32_t symbolFrequency);
  ~QPSK();

  uint8_t getByte();
};

} // namespace SymbolSource
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_QPSK_H_ */