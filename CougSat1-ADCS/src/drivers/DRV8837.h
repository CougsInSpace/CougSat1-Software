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
 * @brief Provides an interface for output to h-bridge 
*/

#ifndef DRV8837_H_
#define DRV8837_H_

#include "mbed.h"
#include "ADCSPins.h"
#include "../tools/CISError.h"


class DRV8837
{
  public:
    DRV8837(PinName forPin, PinName revPin, PinName pSleep);
    uint8_t setOutput(float input);
    uint8_t diable();
    uint8_t wake();
    uint8_t stop();

  private:
    PwmOut forward;
    PwmOut reverse;
    DigitalOut sleep;
};

#endif