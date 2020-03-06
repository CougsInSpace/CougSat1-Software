#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_

#include <stdint.h>

namespace Communications {

class Frame {
public:
  Frame();
  ~Frame();

  void add(uint8_t byte);

  bool isDone();

  uint8_t getNextTXByte();
  uint8_t GetPayloadData(int index);
  uint8_t SetPayloadData(int index, uint8_t data);

private:
  static const size_t MAX_PAYLOAD = 1024 + 2; // Packet + packet header

  uint8_t payloadData[MAX_PAYLOAD];
  size_t  length = 0;

  // TODO add CRC field
};

uint8_t Frame::GetPayloadData(int i) {
  if (i > 0 && i < MAX_PAYLOAD) {
    return payloadData[i];
  }
  return 0;
}

uint8_t Frame::SetPayloadData(int index, uint8_t data) {
  if (index > -1 && index < MAX_PAYLOAD) {
    payloadData[index] = data;
    return 1;
  }
  return 0;
}

} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_ */