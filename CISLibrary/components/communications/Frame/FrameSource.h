#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_

#include <stdio.h>
#include <stdint.h>
#include "Codes/EncodingSingleton.h"

namespace Communications {
namespace Frame{
    
class FrameSource {
public:
  FrameSource();
  ~FrameSource();

  void add(uint8_t byte);

  bool isDone();

// This should be in the sink, not the soure
//  uint8_t getNextTXByte();

private:
  static const size_t MAX_PAYLOAD = 1024 + 2; // Packet + packet header

  void reset();

  void loadPreamble();
  void matchStartOfCode();
  void loadPayloadData();
  void loadCRC();
  void loadEndOfFrame();
  bool loadCode(uint8_t byte);
  inline void nextState(){state = (State_t) ((uint8_t) state + 1);}

  uint8_t payloadData[MAX_PAYLOAD];

  uint8_t curCode;

  //counts the number of bits that already loaded into curCode
  uint8_t bufferedBitsCount;

  //references the position of the first bit that has not been loaded into the buffer
  uint8_t unusedBitsPosition;

  uint8_t codeSize;
  
  /* This is a multi purpose index that can mean different things depending on the State.
   *  it usually counts the occurrences of some key feature or item
   */
  uint8_t index;

  enum class State_t:uint8_t {
    PREAMBLE,
    START_OF_FRAME,
    PAYLOAD,
    CRC,
    END_OF_FRAME,
    DONE
  };
  
  State_t state = State_t::PREAMBLE;

  int8_t runDisparity;

  // TODO add CRC field
};

}// namespace FrameSource
} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_ */