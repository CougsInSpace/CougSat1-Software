#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_H_

#include "tools/CircularBuffer.h"
#include <ResultCode.h>
#include <stdint.h>

namespace Communications {

class IQSource {
public:
  /**
   * @brief Construct a new IQSource object
   *
   */
  IQSource() {}

  /**
   * @brief Destroy the IQSource object
   *
   */
  virtual ~IQSource() {}

  /**
   * @brief Initialize the IQ source,
   *
   * Setup an interrupt to poll ADC
   * Initialize the USB device
   *
   * @return ResultCode_t
   */
  virtual ResultCode_t init() = 0;

  /**
   * @brief Get the IQ data from the source
   *
   * @param dataI buffer
   * @param dataQ buffer
   * @return ResultCode_t
   */
  virtual inline ResultCode_t getIQ(int16_t & dataI, int16_t & dataQ) {
    PairInt16_t  iqPair;
    ResultCode_t result = buf.pop(iqPair);
    dataI               = iqPair.a;
    dataQ               = iqPair.b;
    return result;
  }

protected:
  CircularBuffer<PairInt16_t> buf;
};

} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_H_ */