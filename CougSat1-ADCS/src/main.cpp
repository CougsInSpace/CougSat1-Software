/******************************************************************************
 * Copyright (c) 2019 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file ADCS.cpp
 * @author Ryal O'Neil
 * @date 2019-1-27
 * @brief Starts the ADCS software
 *
 * Initializes ADCS object and starts the eventQueue
 */

#include "drivers/ADCS/ADCS.h"
#include "ADCSPins.h"

int main() 
{
    ADCS ADCS;
    ADCS.initialize();
}