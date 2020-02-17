//
// Created by Hugh on 10/30/2019.
//

#ifndef CISLAYER2_THREEENCODING_H
#define CISLAYER2_THREEENCODING_H


class ThreeEncoding {
    friend class EncodingSingleton;

public:
    inline uint8_t getThreeBitEncoding(){ return threeBitEncoding; }
    inline uint8_t getFourBitEncodingMinus(){ return fourBitEncodingMinus; }
    inline uint8_t getFourBitEncodingPlus(){ return fourBitEncodingPlus; }
    inline uint8_t getFourBitEncoding(bool positive){ return positive ? getFourBitEncodingPlus() : getFourBitEncodingMinus(); }


    inline int8_t getChangeInRunDisparity(bool mode) { return mode ? -changeInRunDisparity : changeInRunDisparity; }
protected:
    ThreeEncoding(uint8_t threeBitEncoding, uint8_t fourBitEncodingMinus);

private:
    uint8_t threeBitEncoding;
    uint8_t fourBitEncodingMinus;
    uint8_t fourBitEncodingPlus;

    /**
     * Represents the change in run disparity from negative to positive
     */
    int8_t changeInRunDisparity;

};


#endif //CISLAYER2_THREEENCODING_H
