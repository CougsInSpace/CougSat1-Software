/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file talkToRcs.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief recieve a packet from the ground through the RCS
 */

#include "events.h"

/**
 * Recieve a packet from the ground
 * Reads address of the message
 * Adds a subsystem message event to route the packet to them
 * @param dataBlock unused, pass in nullptr
 * @param dataLength unused, pass in null
 */
void talkToRCS(uint8_t *dataBlock, uint16_t dataLength){
	//Inform RCS: ready to recieve packet
  //Recieve packet
  //Read address
  //Route packet
}
