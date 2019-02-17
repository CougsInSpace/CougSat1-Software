/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file DRV8837.cpp
 * @author Eric Curtland
 * @date 3 Feburary 2019
 * @brief Provides an interface for output to h-bridge for magnetorquers 
 *
*/

#include "DRV8837.h"

DRV8837::DRV8837(PwmOut &forPin, PwmOut &revPin,Digitalout &pSleep) :forward (forPin),reverse(revPin),sleep(pSleep){
    PwmOut forward(forPin);
    PwmOut reverse(revPin);
    Digitalout sleepPin(pSleep);
    forward.frequency(1000);
    reverse.frequency(1000);

}

unit8_t DRV8837::setCoil(float input){
    if(input > 0){
        forward.write(input);
        reverse.write(0);
    }else{
        reverse.write(input *-1);
        forward.write(0);
    }
}

unit8_t DRV8837::wake(void){
    sleep(HIGH);
    forward.write(0);
    reverse.write(0);
}
unit8_t DRV8837::sleep(void){
    sleep(LOW);
}
unit8_t DRV8837::stop(void){
    forward.write(0);
    reverse.write(0);
}


