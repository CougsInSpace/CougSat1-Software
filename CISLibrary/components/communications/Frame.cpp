#include "Frame.h"

namespace Communications {

/**
 * @brief Construct a new Frame:: Frame object
 *
 */
Frame::Frame() {}

/**
 * @brief Destroy the Frame:: Frame object
 *
 */
Frame::~Frame() {}

/**
 * @brief Add a byte to the frame from the receiver
 *
 *
 * @param byte
 */
void Frame::add(uint8_t byte) {}

/**
 * @brief Check if the frame is complete
 *
 * @return true when the EOF has been received or when the EOF has been got in
 * getNextTXByte
 * @return false otherwise
 */
bool Frame::isDone() {
  return false;
}


size_t Frame::amendCode(uint8_t *buf, uint8_t code6, uint8_t code4, bool start){
static uint8_t bitOffset;
if (start){
  bitOffset = 0;
}
switch (bitOffset) 
{
case 0: 
  *buf = (code6<<2)|(code4>>2);
  *(buf+1) = (code4<<6);
  bitOffset = 2;
  return 1;

case 2: 
  *buf|= code6;
  *(buf+1) = (code4<<4);
  bitOffset = 4;
  return 1;

case 4:
  *buf|= (code6>>2);
  *(buf+1) = (code6<<6)|(code4<<2);
  bitOffset = 6;
  return 1;

case 6:
  *buf|= (code6>>4);
  *(buf+1) = (code6<<4)|(code4);
  bitOffset = 0;
  return 2;//fully filled buf and 1 baby!;

default:
  break;
}
}

/**
 * @brief Get the next byte to transmit
 *
 * @return uint8_t next 8 bits to transmit or END_OF_FILE if no more to transmit
 */
uint8_t Frame::getNextTXByte() {
  static uint8_t encodedFrameBuffer [MAX_FRAME_ENCODED];
  static size_t encodedFrameLength = 0;
  static size_t transmitOffset = 0;



  if (!encoded){
    bool negativeRD = true;
    encodedFrameLength = 0;
    uint8_t *currentBuf = encodedFrameBuffer;
    // generate a bitstream

    // preamble 
    currentBuf += amendCode(currentBuf, CommandCodes6::PREAMBLE_NEG,CommandCodes4::PREAMBLE_NEG, true);
    currentBuf += amendCode(currentBuf, CommandCodes6::PREAMBLE_NEG,CommandCodes4::PREAMBLE_NEG);
    currentBuf += amendCode(currentBuf, CommandCodes6::PREAMBLE_NEG,CommandCodes4::PREAMBLE_NEG);
    currentBuf += amendCode(currentBuf, CommandCodes6::PREAMBLE_NEG,CommandCodes4::PREAMBLE_NEG);
    currentBuf += amendCode(currentBuf, CommandCodes6::PREAMBLE_NEG,CommandCodes4::PREAMBLE_NEG);
    currentBuf += amendCode(currentBuf, CommandCodes6::PREAMBLE_NEG,CommandCodes4::PREAMBLE_NEG);
    currentBuf += amendCode(currentBuf, CommandCodes6::PREAMBLE_NEG,CommandCodes4::PREAMBLE_NEG);

    // start
    currentBuf += amendCode(currentBuf, CommandCodes6::START_NEG,CommandCodes4::START_NEG);


    // payload
    // iterate throught the payload
    // convert payload byte to code
    // first 5 bits - lookup table
    // last 3 bits - lookup table  note edge case (alternate code)
    // if not even RD, flip negativeRD

    // 8, 6+2, 4+4, 6+2, 8,
    
    // encode frame into buffer

    // set encoded frame length


    // reset transmit offset
    transmitOffset = 0;
    encoded = true;
  }

  if (encodedFrameLength == transmitOffset){
    // reached the last code in the buffer
    return END_OF_FILE;
  }

  return encodedFrameBuffer[transmitOffset++];

}

} // namespace Communications