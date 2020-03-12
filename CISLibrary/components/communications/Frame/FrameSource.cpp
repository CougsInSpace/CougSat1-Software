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
  printf("Reset code %d\n", curCode);
  curCode = 0;
  bufferedBitsCount = 0;
  unusedBitsPosition = 0;
  codeSize = 4;
  index = 0;
  state = State_t::PREAMBLE;
  runDisparity = -1;
  wasReset = true;
}


/**
 * @brief Add a byte to the frame from the receiver
 *
 *
 * @param byte
 */
void FrameSource::add(uint8_t byte) {
  wasReset = false;
  while(loadCode(byte) && !wasReset){
    switch (state)
    {
    case State_t::PREAMBLE:
      loadPreamble();
      break;
  
    case State_t::START_OF_FRAME:
      matchStartOfCode();
      break;

    case State_t::PAYLOAD:
      loadPayloadData();
      break;

    case State_t::CRC:
      loadCRC();
      break;

    case State_t::DONE:
      loadEndOfFrame();
      break;
    }
  }
}

void FrameSource::loadPreamble(){
  EncodingSingleton *encoding = EncodingSingleton::getInstance();
  
  if(codeSize == 6){
    FiveEncoding *fiveEncoding = encoding->getControlCodeFiveEncodingFromSix(curCode);
    if(fiveEncoding == nullptr || fiveEncoding->getFiveBitEncoding() != 0b11100){
      printf("Resetting on 3 encoding with index %d and ptr %d\n", index, fiveEncoding);
      reset();
      return;
    }

    int8_t change = fiveEncoding->getRunDisparitychange(runDisparity);
    printf("Change %d\n", change);
    runDisparity += change;

  } else if(codeSize == 4){
    ThreeEncoding *threeEncoding = encoding->getControlCodeThreeEncodingFromFour(curCode, runDisparity);
    if(threeEncoding == nullptr || threeEncoding->getThreeBitEncoding() != 0b101){
      printf("Resetting on 3 encoding with index %d and ptr %d\n", index, threeEncoding);
      reset();
      return;
    }

    int8_t change = threeEncoding->getChangeInRunDisparity(runDisparity);
    printf("Change %d\n", change);
    runDisparity += change;

    index++;
    if(index == 7){
      printf("Loaded the preamble");
      nextState();
    }
  }

  //TODO check run disparity
  return;
}

  bool FrameSource::loadCode(uint8_t code) {
    //Switch from reading 4 to 6 or vice versa
    if(unusedBitsPosition >= 8){
      unusedBitsPosition = 0;
      return false;
    }

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

      printf("Full buffer with %d\n\n", curCode);

      return true;
    } else {
      unusedBitsPosition = 0;
      //Return code size to previous state because the buffer was not fully loaded
      codeSize = codeSize == 6 ? 4 : 6;

      printf("Partial buffer\n\n");

      return false;
    }
  }

void FrameSource::matchStartOfCode(){}

void FrameSource::loadPayloadData(){}

void FrameSource::loadCRC(){}

void FrameSource::loadEndOfFrame(){}

bool FrameSource::isDone(){
  return false;
}

} // namespace FrameSource
} // namespace Communications