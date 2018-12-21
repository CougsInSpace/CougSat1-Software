/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file Events.h
 * @author Bradley Davis
 * @date 7 Nov 2018
 * @brief List of events 
 *
 */

#ifndef _SRC_EVENTS_EVENTS_H_
#define _SRC_EVENTS_EVENTS_H_

#include "mbed.h"

uint8_t eventADC();
uint8_t eventPeriodic();

#endif /* _SRC_EVENTS_EVENTS_H_ */