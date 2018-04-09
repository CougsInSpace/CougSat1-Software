/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file IHU.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Controls all intracommunication on the satellite
 *
 * IHU Tasks:
 *  -Handle communication between all subsystems
 *  -Perform periodic check ups on the entire satellite
 */

#ifndef SRC_SYSTEMINTERFACES_IHU_H_
#define SRC_SYSTEMINTERFACES_IHU_H_

#include <mbed.h>
#include <rtos.h>
#include <filesystem/fat/FATFileSystem.h>

#include "systemInterfaces/PMIC.h"
#include "systemInterfaces/ADCS.h"
#include "systemInterfaces/RCS.h"
#include "systemInterfaces/Payload.h"
#include "systemInterfaces/IFJR.h"
#include "drivers/SDBlockDevice.h"

#define IHU_EVENT_QUEUE_SIZE 20

class IHU {
  public:
    static IHU* getInstance();

    void addEvent(uint8_t eventIndex, uint8_t *dataBlock, uint16_t dataLength);
    void addEventPeriodic(int ms, uint8_t eventIndex, uint8_t *dataBlock,
        uint16_t dataLength);
    void initialize();
    void run();
    void stop();

  private:
    //Singleton class design
    IHU();
    ~IHU();
    IHU(IHU const&);
    void operator=(IHU const&);
    static IHU* instance;

    //System interfaces
    ADCS adcs;
    Payload payload;
    PMIC pmic;
    RCS rcs;
    IFJR ifjr;

    //Hardware drivers
    EventQueue queue;
    I2C i2cPrimary;
    I2C i2cSecondary;
    SPI spi;
    SDBlockDevice sd;
    FATFileSystem fs;
};

#endif /* !SRC_SYSTEMINTERFACES_IHU_H_ */
