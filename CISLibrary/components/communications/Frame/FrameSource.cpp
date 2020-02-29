#include "FrameSource.h"

namespace Communications {
namespace Frame{

/**
 * @brief Construct a new Frame:: Frame object
 *
 */
FrameSource::FrameSource() {
  reset();
}

/**
 * @brief Destroy the Frame:: Frame object
 *
 */
FrameSource::~FrameSource() {}


void FrameSource::reset(){
  length = 0;
  curCode = 0;
  read6 = true;
  index = 0;
  state = State_t::PREAMBLE;
  runDisparity = -1;
}


/**
 * @brief Add a byte to the frame from the receiver
 *
 *
 * @param byte
 */
void FrameSource::add(uint8_t byte) {
  while(loadCode(byte)){
    switch (state)
    {
    case State_t::PREAMBLE:
      loadPreamble(byte);
      break;
  
    case State_t::START_OF_FRAME:
      matchStartOfCode(byte);
      break;

    case State_t::PAYLOAD:
      loadPayloadData(byte);
      break;

    case State_t::CRC:
      loadCRC(byte);
      break;

    case State_t::DONE:
      loadEndOfFrame(byte);
      break;
    }
  }
}

void FrameSource::loadPreamble(uint8_t code){
  return;
}

  bool FrameSource::loadCode(uint8_t code) {
    //Switch from reading 4 to 6 or vice versa
    codeSize = codeSize == 6 ? 4 : 6;

    if(bufferedBitsCount == 0){
      //zero out curCode if nothing is said to be buffered
      curCode = 0;
    }

    //Load bits from code into curCode
    curCode |=  (code << unusedBitsPosition) >> (8 - codeSize + bufferedBitsCount);
    
    //Mask out the bits that are outside of the range of our code size;
    // curCode &= 0b00111111 when code size = 6
    // curCode &= 0b00001111 when code size = 4
    curCode &= (1 << codeSize) - 1;

    uint8_t oldBufferedBitsCount = bufferedBitsCount;
    bufferedBitsCount += (8 - unusedBitsPosition);
    unusedBitsPosition += codeSize - oldBufferedBitsCount;

    // printf("cur code = %d\n", curCode);
    // printf("buffered bits count %d\n", bufferedBitsCount);
    // printf("unusedBitsPosition = %d\n", unusedBitsPosition);
    // printf("old buffered bits count %d", oldBufferedBitsCount);

    //if we have fully populated the buffer 
    if(bufferedBitsCount >= codeSize){
      //Set up buffered bits count for next loadCode on new code
      bufferedBitsCount = 0;

      printf("Full buffer\n\n");

      return true;
    } else {
      unusedBitsPosition = 0;
      //Return code size to previous state because the buffer was not fully loaded
      codeSize = codeSize == 6 ? 4 : 6;

      printf("Partial buffer\n\n");

      return false;
    }
  }

void FrameSource::matchStartOfCode(uint8_t code){}

void FrameSource::loadPayloadData(uint8_t code){}

void FrameSource::loadCRC(uint8_t code){}

void FrameSource::loadEndOfFrame(uint8_t code){}

} // namespace FrameSource
} // namespace Communications