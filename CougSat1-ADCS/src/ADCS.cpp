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
#define CDH_FLAG_MSG_READY 0x1
#define PERIOD_MS_POLLING_SLEEP 1
#define PERIOD_MS_MAIN_SLEEP 1000

/**
 * @brief Function for the polling thread
 * 
 */
void ADCS::monitorThread()
{
    while (true)
    {
        Thread::wait(PERIOD_MS_POLLING_SLEEP);
        //printf("Checking for events!\r\n");
        if (cdh.checkRead())
        {
            cdhRead.signal_set(CDH_FLAG_MSG_READY);
            //printf("Turning on I2C\r\n");
        }
    }
}

/**
 * @brief Function for cdh communication thread
 * 
 */
void ADCS::cdhThread()
{
    while (true)
    {
        Thread::signal_wait(CDH_FLAG_MSG_READY);
        cdh.readCDH();
    }
}

/**
 * @brief Construct a new ADCS::ADCS object
 * 
 */
ADCS::ADCS() : cdh(IHU_ADDRESS, PIN_I2C_BUS_SDA, PIN_I2C_BUS_SCL)
{
    monitor.set_priority(osPriorityRealtime);
    cdhRead.set_priority(osPriorityRealtime);
    cdhRead.start(callback(this, &ADCS::cdhThread));
    monitor.start(callback(this, &ADCS::monitorThread));
}

/**
 * @brief main function for ADCS object
 * 
 */
void ADCS::mainThread()
{
    printf("Outside Main!\r\n");
    while (true)
    {
        printf("Process Main!\r\n");
        Thread::wait(PERIOD_MS_MAIN_SLEEP);
    }
}