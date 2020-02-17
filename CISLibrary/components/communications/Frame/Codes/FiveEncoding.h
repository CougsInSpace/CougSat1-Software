//
// Created by Hugh on 10/30/2019.
//

#ifndef CISLAYER2_FIVEENCODING_H
#define CISLAYER2_FIVEENCODING_H


#include <cstdint>

class FiveEncoding {
    friend class EncodingSingleton;
public:
    inline uint8_t getFiveBitEncoding(){ return fiveBitEncoding; }

    inline uint8_t getSixBitEncodingMinus(){ return sixBitEncodingMinus; }
    inline uint8_t getSixBitEncodingPlus(){ return sixBitEncodingMinus ^ 0b111111; }
    inline uint8_t getSixBitEncoding(bool mode){ return mode ? getSixBitEncodingPlus() : getSixBitEncodingMinus();}

    /***
     * @param curRunDisparity, the current run disparity
     * @return returns the amount that the run disparity has changed after encountering this code with the supplied run disparity
     */
    inline uint8_t getRunDisparitychange(int8_t curRunDisparity) { return curRunDisparity <= 0 ? runDisparityChange : -runDisparityChange; }


protected:
    FiveEncoding(uint8_t fiveBitEncoding, uint8_t sixBitEncodingMinus);

private:
    uint8_t fiveBitEncoding;
    uint8_t sixBitEncodingMinus;
    uint8_t runDisparityChange;
};


#endif //CISLAYER2_FIVEENCODING_H
