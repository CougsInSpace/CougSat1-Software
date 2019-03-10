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
 * @brief Provides an interface for output to h-bridge 
 *
*/

#include "DRV8837.h"
/**
 * @brief Construct a new DRV8837::DRV8837 object
 * 
 * @param forPin pin name for the forward direction
 * @param revPin pin name for the reverse direction
 * @param pSleep pin name of the sleep pin on h-bridge
 */
DRV8837::DRV8837(PinName forPin, PinName revPin, PinName pSleep) : forward(forPin), reverse(revPin), sleep(pSleep)
{
    forward.frequency(1000);
    reverse.frequency(1000);
}
/**
 * @brief 
 * 
 * @param input -1f to 1f to determine percentage duty cycle
 * negative values will result in reversed flow
 * 
 * @return unit8_t for future debuging
 */
unit8_t DRV8837::setCoil(float input)
{
    if (input > 0.0f)
    {
        forward.write(input);
        reverse.write(0.0f);
    }
    else
    {
        reverse.write(input * -1.0f);
        forward.write(0.0f);
    }
}
/**
 * @brief wakes up hbridge takes 30 micro seconds
 * ssets output to hbridge to 0
 * 
 * @return unit8_t for future debuging
 */
unit8_t DRV8837::wake()
{
    sleep(HIGH);
    forward.write(0.0f);
    reverse.write(0.0f);
}
/**
 * @brief puts hbridge to sleep
 * 
 * @return unit8_t for future debuging
 */
unit8_t DRV8837::sleep()
{
    sleep(LOW);
    forward.write(0.0f);
    reverse.write(0.0f);
}
/**
 * @brief sets all hbridge output to zero
 * 
 * @return unit8_t for future debuging
 */
unit8_t DRV8837::stop()
{
    forward.write(0.0f);
    reverse.write(0.0f);
}
