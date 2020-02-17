#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_

#include <stdint.h>

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

  void loadPreamble(uint8_t code);
  void matchStartOfCode(uint8_t code);
  void loadPayloadData(uint8_t code);
  void loadCRC(uint8_t code);
  void loadEndOfFrame(uint8_t code);

  uint8_t payloadData[MAX_PAYLOAD];
  size_t  length;

  uint8_t curCode;
  uint8_t offset;
  bool read6;
  
  /* This is a multi purpose index that can mean different things depending on the State.
   *  it usually counts the occurences of some key feature or item
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