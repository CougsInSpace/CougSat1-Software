/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file rcsMessage.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief sends a message to the RCS via the RCS system interface
 *
 * dataBlock structure:
 * First byte is command
 *
 * The rest of the dataBlock is specific to the command, see
 * resource/CommunicationProtocol for details
 */
#include "IHU.h"
#include "events.h"

/**
 * Sends a message to the RCS
 * @param dataBlock of message, see file header for detailed contents
 * @param dataLength of dataBlock
 */
void rcsMessage(uint8_t *dataBlock, uint16_t dataLength){
  //Read first byte to detrmine choice of action
  //Perfrom action
  //If RCS has data to return, add event "sendToGround"
}
