#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_H_

#include "Frame.h"
#include "Packet.h"
#include "tools/CircularBuffer.h"
#include <stdint.h>

namespace Communications {
namespace IQSource {

class IQSource {
public:
  Packet Lv2_3Ascend(Frame F);   // I'm assuming all checks have been completed
  Frame  Lv3_2Descent(Packet P); // No checks are needed here
  bool   IsEquivalent2_3(
        Frame & F, Packet & P, int size); // This will test the equivlenace of
                                          // packets and frame payload data
  bool Test2_3Ascend(Frame & F);
  bool Test2_3Descend(Packet & P);
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

Packet IQSource::Lv2_3Ascend(
    Frame F) { // I'm assuming all safety tests have been passed.
  int    i = 0;
  Packet P;
  for (i = 0; F.GetPayloadData(i);
       i++) { // Assuming "unoccupied" indexes will be zeroed out.
    P.OverwriteData(F.GetPayloadData(i), i);
  }
  return P;
}

Frame IQSource::Lv3_2Descent(Packet P) {
  Frame F;
  int   i = 0;
  for (i = 0; P.GetData(i); i++) {
    F.SetPayloadData(P.GetData(i), i);
  }
}
bool IQSource::IsEquivalent2_3(Frame & F, Packet & P,
    int size) { // Just test for equivalence in the 1026 data.
  for (int i = 0; i < size; i++) { // Size will be provided
    if (F.GetPayloadData(i) != P.GetData(i)) {
      return false;
    }
  }
  return true;
}

bool IQSource::Test2_3Ascend(Frame & F) { //
  Packet P = Lv2_3Ascend(F);
  int    i = P.GetLength();
  return IsEquivalent2_3(F, P, i);
}
bool IQSource::Test2_3Descend(Packet & P) {
  int   i = P.GetLength();
  Frame F = Lv3_2Descent(P);
  return IsEquivalent2_3(F, P, i);
}
} // namespace IQSource
} // namespace Communications
// I assume the SRC facilitates data ascension
#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_IQ_SOURCE_H_ */