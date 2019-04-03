/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file ADCS.h
 * @author Ryal O'Neil
 * @date 2019-03-24
 * @brief Initializes subsystem objects and controls priority queue
 *
 * ADCS Tasks:
 *	-Initialize subsystems
 *  -Read 4x IMUs
 *  -Read 1x GPS
	-Reach out to hbridge, provide it a PWM duty cycle signal to tell it how much we want it to run and	which direction
	-Read thermistor of magnetorquer
	-Get power info from CDH
	-Photodiodes
 */

#ifndef ADCS_H
#define ADCS_H

#include <mbed.h>
#include <rtos.h>
#include "ADCSPins.h"
#include "components/CDHCOM.h"

class ADCS
{
  private:
	Thread monitor;
	Thread cdhRead;
	char message[4];
	CDHCOM cdh;
	
  public:
	ADCS();
	void monitorThread();
	void cdhThread();
	void mainThread();
};
#endif