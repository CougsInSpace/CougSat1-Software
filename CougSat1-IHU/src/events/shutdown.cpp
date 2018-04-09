/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file shutdown.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief properly powers off satellite
 */
#include "IHU.h"
#include "events.h"

/**
 * Properly powers off satellite
 * @param dataBlock
 * @param dataLength
 */
void shutdown(uint8_t *dataBlock, uint16_t dataLength){
	//Perform a clean shutdown of all non-essential subsystems
	//Close Data lines between non-essential boards
	//Notify power and comms of shutdown
	IHU::getInstance()->stop(); //Asks PMIC to turn our power rail off
}
