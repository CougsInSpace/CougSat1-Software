/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file Payload.h
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates between the IHU and the Payload
 *
 * Current payload:
 *  -Camera
 *  -Radiation sensor
 */

#ifndef SRC_SYSTEMINTERFACES_PAYLOAD_H
#define SRC_SYSTEMINTERFACES_PAYLOAD_H

#include <mbed.h>
#include "Subsystem.h"
#include "drivers/ArduCAM.h"

#define BURST_LENGTH 512

class Payload : public SubSystem {
  public:
    Payload(SPI &spi, I2C &i2c, PinName selectCAM);
    ~Payload();

    uint8_t initialize();
    uint8_t takePicture();
    uint8_t savePicture();

  private:
    ArduCAM arduCAM;

};

#endif /* SRC_SYSTEMINTERFACES_PAYLOAD_H */
