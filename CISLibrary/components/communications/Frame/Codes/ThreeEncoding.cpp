//
// Created by Hugh on 10/30/2019.
//

#include <cstdint>
#include "ThreeEncoding.h"

ThreeEncoding::ThreeEncoding(uint8_t threeBitEncoding, uint8_t fourBitEncodingMinus){
    this->threeBitEncoding = threeBitEncoding;
    this->fourBitEncodingMinus = fourBitEncodingMinus;
    this->fourBitEncodingPlus = fourBitEncodingMinus ^ 0b1111;

    int ones_count = 0;
    int zeroes_count = 0;

    for(uint_fast8_t i = 0; i < 4; i++){
        if((fourBitEncodingPlus ^ (1 << i)) > 0){
            ones_count++;
        } else {
            zeroes_count++;
        }
    }

    changeInRunDisparity = ones_count - zeroes_count;
}
