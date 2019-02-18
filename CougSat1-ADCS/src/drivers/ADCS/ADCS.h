/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file ADCS.cpp
 * @author Cody Sigvartson
 * @date 7 October 2018
 * @brief Controls all intracommunication on the satellite
 *
 * ADCS Tasks:
 *	-Initialize subsystems
 *  -Read 4x IMUs
 *  -Read 1x GPS
	-Reach out to hbridge, provide it a PWM duty cycle signal to tell it how much we want it to run and	which direction
	-Read thermistor of magnetorquer
	-Get power info from IHU
	-Photodiodes?
 */
#ifndef ADCS_H
#define ADCS_H

#include <mbed.h>
#include <rtos.h>
#include "ADCSPins.h"
#include "drivers/IHUCOM/IHUCOM.h"


class ADCS 
{
	private:
		DigitalIn I2CListen;
		Thread MONITOR;
		Thread I2CREAD;
		char message[4];
		IHUCOM Reader;
	public:
		ADCS();
		void monitor_thread();
		void i2c_thread();
		void initialize();
};
#endif