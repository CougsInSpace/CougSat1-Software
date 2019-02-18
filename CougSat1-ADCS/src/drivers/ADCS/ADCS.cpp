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

void ADCS::monitor_thread() {
    while (true) {
        Thread::wait(1);
        // printf("Checking for events!\r\n");
        if(I2CListen == 1) {
            I2CREAD.signal_set(0x1);
            printf("Turning on I2C\r\n");
        }
    }
}
 
void ADCS::i2c_thread() {
    while(true) {
        Thread::signal_wait(0x1);
        Reader.readIHU();
    }

}

ADCS::ADCS(): I2CListen(PIN_I2C_LISTENING)
{
    MONITOR.set_priority(osPriorityRealtime);
    I2CREAD.set_priority(osPriorityRealtime);
    I2CREAD.start(callback(this, &ADCS::i2c_thread));
    MONITOR.start(callback(this, &ADCS::monitor_thread));
}

void ADCS::initialize() {
    printf("Outside Main!\r\n");
    while (true) {
        printf("Process Main!\r\n");
        Thread::wait(500);
    }
}