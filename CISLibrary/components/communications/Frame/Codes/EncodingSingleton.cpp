//
// Created by Hugh on 10/30/2019.
//

#include <stdexcept>
#include "EncodingSingleton.h"

EncodingSingleton *EncodingSingleton::instance = nullptr;

/***
 * This method is to retrieve information about the symbols D.x
 * @param _sixEncoding the six encoding that you wish to retrieve information about
 * @return nullptr if the _sixEncoding does not exist, otherwise a FiveEncoding ptr
 *      that points to the corresponding FiveEncoding object.
 */
FiveEncoding* EncodingSingleton::getFiveEncodingFromSix(uint8_t _sixEncoding){
    if(_sixEncoding >= sixSize || _sixEncoding < 0){
        return nullptr;
    }

    return sixEncoding[_sixEncoding];
}

/***
 * This method is to retrieve information about the symbols D.x.y
 * @param _fourEncoding the four encoding that you wish to retrieve information about
 * @return nullptr if the _fourEncoding does not exist, otherwise a ThreeEncoding ptr
 *      that points to the corresponding ThreeEncoding object.
 */
ThreeEncoding *EncodingSingleton::getThreeEncodingFromFour(uint8_t _fourEncoding) {
    if(_fourEncoding >= fourSize || _fourEncoding < 0){
        return nullptr;
    }

    return fourEncoding[_fourEncoding];
}

/***
 * This method is to retrieve information about the symbols D.x
 * @param _fiveEncoding the five encoding that you wish to retrieve information about
 * @return null ptr if the encoding does not exist, otherwise a FiveEncoding ptr
 *      that points to the corresponding FiveEncoding object.
 */
FiveEncoding* EncodingSingleton::getFiveEncoding(uint8_t _fiveEncoding) {
    if(_fiveEncoding >= fiveSize || _fiveEncoding < 0) {
        return nullptr;
    }

    return fiveEncoding[_fiveEncoding];
}

/***
 * This method is to retrieve information about the symbols D.x.y
 * @param _threeEncoding the three encoding that you wish to retrieve information about.
 *      Use an input of 7 to retrieve information about D.x.P7
 *      Use an input of 8 to retrieve informaiton about D.x.A7
 * @return nullptr if the encoding does not exists, otherwise a ThreeEncoding ptr
 *      that points to the corresponding ThreeEncoding object.
 */
ThreeEncoding* EncodingSingleton::getThreeEncoding(uint8_t  _threeEncoding) {
    if(_threeEncoding >= threeSize){
        return nullptr;
    }

    return threeEncoding[_threeEncoding];
}

EncodingSingleton* EncodingSingleton::getInstance() {
    if (instance == nullptr ){
        instance = new EncodingSingleton();
    }

    return instance;
}

void EncodingSingleton::makeFives() {
    fiveEncoding[ 0] = new FiveEncoding( 0, 0b100111);
    fiveEncoding[ 1] = new FiveEncoding( 1, 0b011101);
    fiveEncoding[ 2] = new FiveEncoding( 2, 0b101101);
    fiveEncoding[ 3] = new FiveEncoding( 3, 0b110001);
    fiveEncoding[ 4] = new FiveEncoding( 4, 0b110101);
    fiveEncoding[ 5] = new FiveEncoding( 5, 0b101001);
    fiveEncoding[ 6] = new FiveEncoding( 6, 0b011001);
    fiveEncoding[ 7] = new FiveEncoding( 7, 0b111000);
    fiveEncoding[ 8] = new FiveEncoding( 8, 0b111001);
    fiveEncoding[ 9] = new FiveEncoding( 9, 0b100101);
    fiveEncoding[10] = new FiveEncoding(10, 0b010101);
    fiveEncoding[11] = new FiveEncoding(11, 0b110100);
    fiveEncoding[12] = new FiveEncoding(12, 0b001101);
    fiveEncoding[13] = new FiveEncoding(13, 0b101100);
    fiveEncoding[14] = new FiveEncoding(14, 0b011100);
    fiveEncoding[15] = new FiveEncoding(15, 0b010111);
    fiveEncoding[16] = new FiveEncoding(16, 0b011011);
    fiveEncoding[17] = new FiveEncoding(17, 0b100011);
    fiveEncoding[18] = new FiveEncoding(18, 0b010011);
    fiveEncoding[19] = new FiveEncoding(19, 0b110010);
    fiveEncoding[20] = new FiveEncoding(20, 0b011010);
    fiveEncoding[21] = new FiveEncoding(21, 0b101010);
    fiveEncoding[22] = new FiveEncoding(22, 0b011010);
    fiveEncoding[23] = new FiveEncoding(23, 0b111010);
    fiveEncoding[24] = new FiveEncoding(24, 0b110011);
    fiveEncoding[25] = new FiveEncoding(25, 0b100110);
    fiveEncoding[26] = new FiveEncoding(26, 0b010110);
    fiveEncoding[27] = new FiveEncoding(27, 0b110110);
    fiveEncoding[28] = new FiveEncoding(28, 0b001110);
    fiveEncoding[29] = new FiveEncoding(29, 0b101110);
    fiveEncoding[30] = new FiveEncoding(30, 0b011110);
    fiveEncoding[31] = new FiveEncoding(31, 0b101011);

    for(auto & i : sixEncoding){
        i = nullptr;
    }

    for(auto cur : fiveEncoding){
        sixEncoding[cur->getSixBitEncodingMinus()] = cur;
        sixEncoding[cur->getSixBitEncodingPlus()] = cur;
    }
}

EncodingSingleton::EncodingSingleton() {
    makeFives();
    makeThrees();
    makeControlFive();
    makeControlThree();
}

void EncodingSingleton::makeThrees() {
    threeEncoding[0] = new ThreeEncoding(0, 0b1011);
    threeEncoding[1] = new ThreeEncoding(1, 0b1001);
    threeEncoding[2] = new ThreeEncoding(2, 0b0101);
    threeEncoding[3] = new ThreeEncoding(3, 0b1100);
    threeEncoding[4] = new ThreeEncoding(4, 0b1010);
    threeEncoding[5] = new ThreeEncoding(5, 0b1110);
    threeEncoding[6] = new ThreeEncoding(6, 0b1110);
    threeEncoding[7] = new ThreeEncoding(7, 0b1110);
    threeEncoding[8] = new ThreeEncoding(7, 0b0111);

    for(auto & i : fourEncoding){
        i = nullptr;
    }

    for(auto cur : threeEncoding){
        threeEncoding[cur->getFourBitEncodingMinus()] = cur;
        threeEncoding[cur->getFourBitEncodingPlus()] = cur;
    }
}

void EncodingSingleton::makeControlFive() {
    for(auto &i : controlCodeFiveEncoding){
        i = nullptr;
    }

    controlCodeFiveEncoding[23] = new FiveEncoding(0b10111, 0b111010);
    controlCodeFiveEncoding[27] = new FiveEncoding(0b11011, 0b110110);
    controlCodeFiveEncoding[28] = new FiveEncoding(0b11100, 0b001111);
    controlCodeFiveEncoding[29] = new FiveEncoding(0b11101, 0b101110);
    controlCodeFiveEncoding[30] = new FiveEncoding(0b11110, 0b011110);

    for(auto &i : controlCodeSixEncoding){
        i = nullptr;
    }

    for(auto & i : controlCodeFiveEncoding){
        if(i != nullptr){
            controlCodeSixEncoding[i->getSixBitEncodingMinus()] = i;
            controlCodeSixEncoding[i->getSixBitEncodingPlus()] = i;
        }
    }
}

void EncodingSingleton::makeControlThree() {
    for(auto & i : controlCodeThreeEncoding){
        i = nullptr;
    }

    controlCodeThreeEncoding[0] = new ThreeEncoding(0b000, 0b1011);
    controlCodeThreeEncoding[1] = new ThreeEncoding(0b001, 0b0110);
    controlCodeThreeEncoding[2] = new ThreeEncoding(0b010, 0b1010);
    controlCodeThreeEncoding[3] = new ThreeEncoding(0b011, 0b1100);
    controlCodeThreeEncoding[4] = new ThreeEncoding(0,0);
    controlCodeThreeEncoding[5] = new ThreeEncoding(0,0);
    controlCodeThreeEncoding[6] = new ThreeEncoding(0,0);
    controlCodeThreeEncoding[7] = new ThreeEncoding(0,0);
}
