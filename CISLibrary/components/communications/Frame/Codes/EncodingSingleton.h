//
// Created by Hugh on 10/30/2019.
//

#ifndef CISLAYER2_ENCODINGSINGLETON_H
#define CISLAYER2_ENCODINGSINGLETON_H

#include <stdio.h>

#include "FiveEncoding.h"
#include "ThreeEncoding.h"

class EncodingSingleton {
public:
    static EncodingSingleton* getInstance();
    FiveEncoding* getFiveEncodingFromSix(uint8_t _sixEncoding);
    FiveEncoding* getFiveEncoding(uint8_t _fiveEncoding);

    FiveEncoding *getControlCodeFiveEncoding(uint8_t _fiveEncoding);
    FiveEncoding *getControlCodeFiveEncodingFromSix(uint8_t _sizeEncoding);

    ThreeEncoding* getThreeEncoding(uint8_t _threeEncoding);
    ThreeEncoding* getThreeEncodingFromFour(uint8_t _fourEncoding);

    ThreeEncoding* getControlCodeThreeEncoding(uint8_t _threeEncoding);
    ThreeEncoding* getControlCodeThreeEncodingFromFour(uint8_t _fourEncoding, int8_t runDisparity);

private :
    static const int fiveSize = 32;
    static const int sixSize = 59;
    static const int threeSize = 9;
    static const int fourSize = 15;

    static EncodingSingleton *instance;
    FiveEncoding *fiveEncoding[fiveSize];
    FiveEncoding *sixEncoding[sixSize];
    ThreeEncoding *threeEncoding[threeSize];
    ThreeEncoding *fourEncoding[fourSize];

    static const int controlFiveSize = 37;
    static const int controlSixSize = 27;
    static const int controlThreeSize = 8;
    static const int controlFourSize = 14;

    FiveEncoding *controlCodeFiveEncoding[controlFiveSize];
    FiveEncoding *controlCodeSixEncoding[controlSixSize];
    ThreeEncoding *controlCodeThreeEncoding[controlThreeSize];
    ThreeEncoding *controlCodeFourEncodingPositive[controlFourSize];
    ThreeEncoding *controlCodeFourEncodingNegative[controlFourSize];

    __declspec(noinline) void makeFives();
    __declspec(noinline) void makeThrees();
    __declspec(noinline) void makeControlFive();
    __declspec(noinline) void makeControlThree();

    EncodingSingleton();
};

#endif //CISLAYER2_ENCODINGSINGLETON_H
