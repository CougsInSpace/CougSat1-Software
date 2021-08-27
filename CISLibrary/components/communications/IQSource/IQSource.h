#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_H_

#include "../tools/CircularBuffer.h"
#include <cstdint>

namespace Communications {
namespace IQSource {

class IQSource {
public:
  /**
   * @brief Construct a new IQSource object
   *
   * @param sampleFrequency
   */
  IQSource(const uint32_t sampleFrequency) : sampleFrequency(sampleFrequency) {}

  /**
   * @brief Destroy the IQSource object
   *
   */
  virtual ~IQSource() {}

  /**
   * @brief Get the IQ data from the source
   *
   * @param dataI buffer
   * @param dataQ buffer
   */
  virtual void getIQ(int16_t & dataI, int16_t & dataQ) {
    PairInt16_t iqPair;
    while (!iqBuffer.pop(iqPair))
      ; // Block until element is popped
    dataI = iqPair.a;
    dataQ = iqPair.b;
  }

protected:
  CircularBuffer<PairInt16_t> iqBuffer;
  const uint32_t              sampleFrequency;
};

} // namespace IQSource
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_H_ */