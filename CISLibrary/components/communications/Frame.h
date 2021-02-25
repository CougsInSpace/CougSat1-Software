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
  static const uint8_t END_OF_FILE = 0;


private:
  // Packet + packet header
  static const size_t MAX_PAYLOAD = 1024 + 2; 
  // Preamble + Start of Frame + Payload + CRC + End of Frame
  static const size_t MAX_FRAME_DECODED = 7 + 1 + MAX_PAYLOAD + 2 + 1;
  // Max Frame Decoded * 10 bits/code + 1 to round up
  static const size_t MAX_FRAME_ENCODED = MAX_FRAME_DECODED * 10/8 + 1; 

  // static const uint16_t START_CODE;

struct Code {
  uint8_t negativeCode;
  uint8_t positiveCode;
  bool evenRD;
  // TODO consider bit packing
};

static const Code CODES_56[] = {
  {0b100111, 0b011000, false},
  {0b011101, 0b100010, false},
  {0b101101, 0b010010, false},
  {0b110001, 0b110001, true},
  {0b110101, 0b001010, false},
  {0b101001, 0b101001, true},
  {0b011001, 0b101001, true},
  {0b111000, 0b000111, false},
  {0b111001, 0b000110, false},
  {0b100101, 0b100101, true},
  {0b010101, 0b010101, true},
  {0b110100, 0b110100, true},
  {0b001101, 0b110100, true},
  {0b101100, 0b101100, true},
  {0b011100, 0b011100, true},
  {0b010111, 0b101000, false},
  {0b011011, 0b100100, false},
  {0b100011, 0b100011, true},
  {0b010011, 0b010011, true},
  {0b110010, 0b110010, true},
  {0b001011, 0b001011, true},
  {0b101010, 0b101010, true},
  {0b011010, 0b011010, true},
  {0b111010, 0b000101, false},
  {0b110011, 0b001100, false},
  {0b100110, 0b100110, true},
  {0b010110, 0b100110, true},
  {0b110110, 0b001001, false},
  {0b001110, 0b001110, true},
  {0b101110, 0b010001, false},
  {0b011110, 0b100001, false},
  {0b101011, 0b010100, false}
};

static const Code CODES_34[] = {
  {0b1011, 0b0100, false},
  {0b1001, 0b1001, true},
  {0b0101, 0b0101, true},
  {0b1100, 0b0011, false},
  {0b1101, 0b0011, false},
  {0b1010, 0b1010, true},
  {0b0110, 0b0110, true},
  {0b1110, 0b0001, false},
  {0b0111, 0b1000, false}
};

static const enum class CommandCodes6: uint8_t{
  PREAMBLE_NEG = 0b001111,
  PREAMBLE_POS = 0b110000,

  START_NEG = 0b111010,
  START_POS = 0b000101,

  END_NEG = 0b110110,
  END_POS = 0b001001
};

static const enum class CommandCodes4: uint8_t{
  PREAMBLE_NEG = 0b1010,
  PREAMBLE_POS = 0b0101,

  START_NEG = 0b1000,
  START_POS = 0b0111,

  END_NEG = 0b1000,
  END_POS = 0b0111
};

size_t amendCode(uint8_t *buf, uint8_t code6, uint8_t code4, bool start = false);

/*   enum class State: uint8_t{
    START,
    PAYLOAD_DATA,
    CRC_1,
    CRC_2,
    END
  };
 */

  uint8_t payloadData[MAX_PAYLOAD];
  size_t  length = 0;

  bool encoded = false;

  // TODO add CRC field
};

} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_FRAME_H_ */