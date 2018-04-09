/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file events.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Definition for cisEventList array
 */
#include "events.h"

void (*cisEventList[EVENTS_SIZE])(uint8_t *dataBlock, uint16_t dataLength) = {
		ihuPeriodic,
		talkToRCS,
		pmicMessage,
		adcsMessage,
		rcsMessage,
		ihuMessage,
		ifjrMessage,
		sendToGround
};
