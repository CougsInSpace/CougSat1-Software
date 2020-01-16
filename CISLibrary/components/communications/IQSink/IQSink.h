#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SINK_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SINK_H_

#include "tools/CircularBuffer.h"
#include <stdint.h>

namespace Communications {
namespace IQSink {

class IQSink {
public:
  /**
   * @brief Construct a new IQSink object
   *
   */
  IQSink() {}

  /**
   * @brief Destroy the IQSink object
   *
   */
  virtual ~IQSink() {}

  /**
   * @brief Get the IQ data from the source
   *
   * @param dataI buffer
   * @param dataQ buffer
   */
  virtual inline void addIQ(int16_t dataI, int16_t dataQ) = 0;
};

} // namespace IQSink
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SINK_H_ */