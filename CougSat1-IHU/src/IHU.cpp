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

#include <mbed.h>
#include <rtos.h>
#include "IHU.h"
#include "IHUPins.h"
#include "src/events/events.h"
#include "tools/CISError.h"
#include "tools/CISConsole.h"

extern void (*cisEventList[EVENTS_SIZE])(uint8_t *dataBlock,
    uint16_t dataLength);

IHU* IHU::instance = NULL;

/**
 * Instantiate hardware classes and subsystems
 */
IHU::IHU() :
    queue(IHU_EVENT_QUEUE_SIZE * EVENTS_EVENT_SIZE), spi(SPI_MOSI, SPI_MISO,
    SPI_SCLK), i2cPrimary(I2C0_SDA, I2C0_SCL), i2cSecondary(I2C1_SDA, I2C1_SCL), sd(
        PC_3, PC_2, PD_1, PC_1), fs("sd", &sd), adcs(i2cPrimary), payload(spi,
        i2cSecondary, SPI_CS_CAM0), pmic(i2cPrimary), rcs(), ifjr() {
}

/**
 * Deconstuctor is currently empty
 */
IHU::~IHU() {

}

/**
 * Singleton object pointer, creates one if null
 * @return pointer to the singleton object
 */
IHU* IHU::getInstance() {
  if (instance == NULL) {
    instance = new IHU();
  }
  return instance;
}

/**
 * Adds an event to the eventqueue
 * @param eventIndex, see events.h for constants
 * @param dataBlock to give to event
 * @param dataLength of the data block
 */
void IHU::addEvent(uint8_t eventIndex, uint8_t *dataBlock,
    uint16_t dataLength) {
  queue.call(cisEventList[eventIndex], dataBlock, dataLength);
}

/**
 * Adds an event to the event queue periodically
 * @param ms (milliseconds) between additions
 * @param eventIndex, see events.h for constants
 * @param dataBlock to give to event
 * @param dataLength of the data block
 */
void IHU::addEventPeriodic(int ms, uint8_t eventIndex, uint8_t *dataBlock,
    uint16_t dataLength) {
  queue.call_every(ms, cisEventList[eventIndex], dataBlock, dataLength);
}

/**
 * Initializes IHU hardware drivers and system interfaces
 */
void IHU::initialize() {
  CONSOLE_CLR
  ;
  DEBUG("IHU", "Starting initialization");
  CONSOLE_TX("IHU", "Starting initialization");
  uint8_t result;
  i2cPrimary.frequency(400000);
  spi.frequency(1000000);

  result = adcs.initialize();
  if (result != ERROR_SUCCESS) {
    DEBUG("IHU", "ADCS initialization error: 0x%02x", result);
    CONSOLE_TX("IHU", "ADCS initialization error: 0x%02x", result);
    while (true) {
      //Give up
    }
  }

  result = payload.initialize();
  if (result != ERROR_SUCCESS) {
    DEBUG("IHU", "Payload initialization error: 0x%02x", result);
    CONSOLE_TX("IHU", "Payload initialization error: 0x%02x", result);
    while (true) {
      //Give up
    }
  }

  result = pmic.initialize();
  if (result != ERROR_SUCCESS) {
    DEBUG("IHU", "PMIC initialization error: 0x%02x", result);
    CONSOLE_TX("IHU", "PMIC initialization error: 0x%02x", result);
    while (true) {
      //Give up
    }
  }

  result = rcs.initialize();
  if (result != ERROR_SUCCESS) {
    DEBUG("IHU", "RCS initialization error: 0x%02x", result);
    CONSOLE_TX("IHU", "RCS initialization error: 0x%02x", result);
    while (true) {
      //Give up
    }
  }

  result = ifjr.initialize();
  if (result != ERROR_SUCCESS) {
    DEBUG("IHU", "RCS initialization error: 0x%02x", result);
    CONSOLE_TX("IHU", "RCS initialization error: 0x%02x", result);
    while (true) {
      //Give up
    }
  }

  result = sd.init();
  if (result != ERROR_SUCCESS) {
    DEBUG("IHU", "SD card initialization error: 0x%02x", result);
    CONSOLE_TX("IHU", "SD card initialization error: 0x%02x", result);
    while (true) {
      //Give up
    }
  }
  DEBUG("IHU", "Initialization complete");
  CONSOLE_TX("IHU", "Initialization complete");
}

/**
 * Begins an infinite loop of processing the eventQueue
 */
void IHU::run() {
  queue.dispatch();
}

/**
 * Requests PMIC to shutdown the system
 * The event "shutdown" handles telling every system that IHU is turning off
 */
void IHU::stop() {
  //pmic.switchRail(RAIL_IHU, RAIL_OFF);
  return;
}
