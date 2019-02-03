/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file MAGNETORQUER.h
 * @author Eric Curtland
 * @date 3 Feburary 2019
 * @brief Provides an interface for output to magnetorquers 
 *
*/

#include "MAGNETORQUER.h"

MAGNETORQUER::MAGNETORQUER(PinName forPin, PinName revPin,PinName pSleep){
    PwmOut forward(forPin);
    PwmOut reverse(revPin);
    Digitalout sleepPin(pSleep);
    
}

MAGNETORQUER::setCoil(float input){
    if(input > 0){
        forward.write(input);
    }else{
        reverse.write(input *-1);
    }
}

MAGNETORQUER::wake(void){
    sleepPin(HIGH);
}
MAGNETORQUER::sleep(void){
    sleepPin(LOW);
}

