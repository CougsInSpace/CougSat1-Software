//
// Created by Hugh on 10/30/2019.
//

#ifndef CISLAYER2_ENCODINGSINGLETON_H
#define CISLAYER2_ENCODINGSINGLETON_H

#include "FiveEncoding.h"
#include "ThreeEncoding.h"

class EncodingSingleton {
public:
    static EncodingSingleton* getInstance();
    FiveEncoding* getFiveEncodingFromSix(uint8_t _sixEncoding);
    FiveEncoding* getFiveEncoding(uint8_t _fiveEncoding);

    ThreeEncoding* getThreeEncodingFromFour(uint8_t _fourEncoding);
    ThreeEncoding* getThreeEncoding(uint8_t _threeEncoding);

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
    static const int controlFourSize = 7;

    FiveEncoding *controlCodeFiveEncoding[controlFiveSize];
    FiveEncoding *controlCodeSixEncoding[controlSixSize];
    ThreeEncoding *controlCodeThreeEncoding[controlThreeSize];
    ThreeEncoding *controlCodeFourEncoding[controlFourSize];

    void makeFives();
    void makeThrees();
    void makeControlFive();

    EncodingSingleton();

    void makeControlThree();
};

#endif //CISLAYER2_ENCODINGSINGLETON_H
