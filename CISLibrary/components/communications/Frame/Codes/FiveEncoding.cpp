//
// Created by Hugh on 10/30/2019.
//

#include "FiveEncoding.h"

FiveEncoding::FiveEncoding(uint8_t fiveBitEncoding, uint8_t sixBitEncodingMinus) {
    this->fiveBitEncoding = fiveBitEncoding;
    this->sixBitEncodingMinus = sixBitEncodingMinus;

    int ones_count = 0;
    int zeroes_count = 0;
    for(uint_fast8_t i = 0; i < 6; i++){
            if((sixBitEncodingMinus ^ (1 << i)) > 0){
               ones_count++;
            } else {
                zeroes_count++;
            }
    }

    runDisparityChange = ones_count - zeroes_count;
}
