#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_

#include <cstdint>
#include <cstddef>

namespace Communications {

class Frame {
public:
  Frame();
  ~Frame();

  void add(uint8_t byte);

  bool isDone();

  uint8_t getNextTXByte();

private:
  static const size_t MAX_PAYLOAD = 1024 + 2; // Packet + packet header

  uint8_t payloadData[MAX_PAYLOAD];
  size_t  length = 0;

  // TODO add CRC field
};

} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_ */