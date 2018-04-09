/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file sendToGround.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief sends a data packet/file to the ground
 */
#include "IHU.h"
#include "events.h"

/**
 * Sends a data packet/file to the ground
 * @param dataBlock of message, see file header for detailed contents
 * @param dataLength of dataBlock
 */
void sendToGround(uint8_t *dataBlock, uint16_t dataLength){
  //Creates a packet from raw data, possible splitting into smaller packets
  //Sends packet(s) to the RCS for transmission
}
