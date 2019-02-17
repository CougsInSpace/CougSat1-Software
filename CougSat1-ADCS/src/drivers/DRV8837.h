/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file DRV8837.h
 * @author Eric Curtland
 * @date 27 January 2019
 * @brief Provides an interface for output to h-bridge for magnetorquers 
 *
*/

#ifndef DRV8837_H_
#define DRV8837_H_

#include <mbed.h>
#include "ADCSPins.h"



class DRV8837{
    public:

    DRV8837(PinName forPin, PinName revPin,PinName pSleep);
    ~DRV8837();

    // input should be between +- 100 corespending to persentage power
    // sign indicates direction
    unit8_t setCoil(float input);

    unit8_t setCoil(float input, float time);

    //puts h-brige to sleep 
    unit8_t sleep();
    //wakes up h-brige (takes 30 micro seconds)
    unit8_t wake();

    //sets both coils to 0 without puting the hbridge to sleep
    unit8_t stop();

    private:
    PwmOut &forPin;
    PwmOut &revPin;
    Digitalout &pSleep;
  

}


#endif 