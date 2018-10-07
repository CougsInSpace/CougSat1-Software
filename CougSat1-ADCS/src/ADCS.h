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
 *  -Read 4x IMUs
 *  -Read 1x GPS
	-Reach out to hbridge, provide it a PWM duty cycle signal to tell it how much we want it to run and	which direction
	-Read thermistor of magnetorquer
	-Get power info from IHU
	-Photodiodes?
 */

#ifndef SRC_SYSTEMINTERFACES_ADCS_H_
#define SRC_SYSTEMINTERFACES_ADCS_H_

#include <mbed.h>
#include <rtos.h>
#include <filesystem/fat/FATFileSystem.h>

#include "systemInterfaces/PMIC.h"
#include "systemInterfaces/IHU.h"
#include "systemInterfaces/RCS.h"
#include "systemInterfaces/Payload.h"
#include "systemInterfaces/IFJR.h"
#include "drivers/SDBlockDevice.h"
#include "GPS.h"

#define ADCS_EVENT_QUEUE_SIZE 20

class ADCS {
  public:
    static ADCS* getInstance();

    void addEvent(uint8_t eventIndex, uint8_t *dataBlock, uint16_t dataLength);
    void addEventPeriodic(int ms, uint8_t eventIndex, uint8_t *dataBlock,
        uint16_t dataLength);
    void initialize();
    void run();
    void stop();

  private:
    //Singleton class design
    ADCS();
    ~ADCS();
    ADCS(ADCS const&);
    void operator=(ADCS const&);
    static ADCS* instance;

    //System interfaces
	IHU ihu;
    PMIC pmic;
	GPS gps;

    //Hardware drivers
    EventQueue queue;
    I2C i2cPrimary;
    I2C i2cSecondary;
    SPI spi;
    SDBlockDevice sd;
    FATFileSystem fs;
};

#endif /* !SRC_SYSTEMINTERFACES_ADCS_H_ */
