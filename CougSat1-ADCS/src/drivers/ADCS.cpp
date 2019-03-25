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

#include "ADCS.h"
#define CDHFLAG 0x1
#define ONEMILLISECOND 1
#define ONESECOND 1000

/**
 * @brief Function for the polling thread
 * 
 */
void ADCS::monitor_thread()
{
    while (true)
    {
        Thread::wait(ONEMILLISECOND);
        //printf("Checking for events!\r\n");
        if (CDH.checkRead())
        {
            CDHREAD.signal_set(CDHFLAG);
            //printf("Turning on I2C\r\n");
        }
    }
}

/**
 * @brief Function for cdh communication thread
 * 
 */
void ADCS::cdh_thread()
{
    while (true)
    {
        Thread::signal_wait(CDHFLAG);
        CDH.readCDH();
    }
}

/**
 * @brief Construct a new ADCS::ADCS object
 * 
 */
ADCS::ADCS() : CDH(IHU_ADDRESS, PIN_I2C_BUS_SDA, PIN_I2C_BUS_SCL)
{
    MONITOR.set_priority(osPriorityRealtime);
    CDHREAD.set_priority(osPriorityRealtime);
    CDHREAD.start(callback(this, &ADCS::cdh_thread));
    MONITOR.start(callback(this, &ADCS::monitor_thread));
}

/**
 * @brief main function for ADCS object
 * 
 */
void ADCS::main()
{
    printf("Outside Main!\r\n");
    while (true)
    {
        printf("Process Main!\r\n");
        Thread::wait(ONESECOND);
    }
}