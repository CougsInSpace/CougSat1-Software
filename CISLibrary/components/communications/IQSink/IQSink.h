#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SINK_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SINK_H_

#include "../tools/CircularBuffer.h"
#include <cstdint>

namespace Communications {
namespace IQSink {

class IQSink {
public:
  /**
   * @brief Construct a new IQSink object
   *
   * @param sampleFrequency
   */
  IQSink(const uint32_t sampleFrequency) : sampleFrequency(sampleFrequency) {}

  /**
   * @brief Destroy the IQSink object
   *
   */
  virtual ~IQSink() {}

  /**
   * @brief Add IQ to the sink
   *
   * @param dataI buffer
   * @param dataQ buffer
   */
  virtual void addIQ(int16_t dataI, int16_t dataQ) = 0;

  /**
   * @brief Get the sample frequency of the sink
   *
   * @return uint32_t samples per second
   */
  uint32_t getSampleFrequency() const {
    return sampleFrequency;
  }

protected:
  const uint32_t sampleFrequency;
};

} // namespace IQSink
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SINK_H_ */