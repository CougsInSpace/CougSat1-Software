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

  while(hasNextCode()){
    loadCode(byte);

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

  bufferCode(byte);
  }
}

void FrameSource::loadPreamble(uint8_t code){
  return;
}

bool FrameSource::hasNextCode(){
  //TODO make this work
  return true;
}

void FrameSource::bufferCode(uint8_t code){
  uint8_t unbufferedBitsCount = 8 - bufferedBitsCount;
  if(codeSize == 6){

    //shift the first non used bit to the beggining of our code variable
    if(unbufferedBitsCount == 2){
      code = 0x00001111 & code << 2;
    } else {
      code = 0x00001111 & code >> (unbufferedBitsCount - 4);
    }

    bufferedBitsCount = 

        
    
    curCode = (code >> 4);

    bufferedBitsCount = 4 - bufferedBitsCount;

    codeSize = 4;
  } else {

    codeSize = 6;
  }

  read6 = !read6;
}

void FrameSource::matchStartOfCode(uint8_t code){}

void FrameSource::loadPayloadData(uint8_t code){}

void FrameSource::loadCRC(uint8_t code){}

void FrameSource::loadEndOfFrame(uint8_t code){}

/**
 * @brief Check if the frame is complete
 *
 * @return true when the EOF has been received or when the EOF has been got in
 * getNextTXByte
 * @return false otherwise
 */
bool FrameSource::isDone() {
  return state == State_t::DONE;
}

// /**
//  * @brief Get the next byte to transmit
//  *
//  * @return uint8_t
//  */
// uint8_t FrameSource::getNextTXByte() {
//   return -1;
// }

} // namespace FrameSource
} // namespace Communications