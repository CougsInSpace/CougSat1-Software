#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_QPSK_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_QPSK_H_

#include "SymbolSource.h"

#include <ResultCode.h>
#include <stdint.h>

namespace Communications {
namespace SymbolSource {

class QPSK : public SymbolSource {
public:
  QPSK(const QPSK &) = delete;
  QPSK & operator=(const QPSK &) = delete;

  QPSK(const uint32_t symbolFrequency);
  ~QPSK();

  ResultCode_t init();

  inline ResultCode_t getByte(uint8_t & byte);
};

} // namespace SymbolSource
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_SYMBOL_SOURCE_QPSK_H_ */