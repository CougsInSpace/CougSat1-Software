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
  offset = 0;
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

void FrameSource:loadPreamble(uint8_t code){

}

uint8_t FrameSource:loadBit(uint8_t code){
  curCode |= code >> (offset);
  if(read6){
    offset = (offset + 6) % 8;

  } else {
    offset = (offset + 4) % 8;
  }
  




  read6 = !read6;


}

void FrameSource:matchStartOfCode(uint8_t code){}

void FrameSource:loadPayloadData(uint8_t code){}

void FrameSource:loadCRC(uint8_t code){}

void FrameSource:loadEndOfFrame(uint8_t code){}

/**
 * @brief Check if the frame is complete
 *
 * @return true when the EOF has been received or when the EOF has been got in
 * getNextTXByte
 * @return false otherwise
 */
bool FrameSource::isDone() {
  return stage == 5;
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