/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file Payload.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates between the IHU and the Payload
 *
 * Current payload:
 *  -Camera
 *  -Radiation sensor
 */

#include "Payload.h"
#include "drivers/CSRI.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

/**
 * Constructs payload object
 * @param spi connected to payload
 * @param i2c connected to payload
 * @param selectCAM pin for camera SPI select
 */
Payload::Payload(SPI &spi, I2C &i2c, PinName selectCAM) :
    arduCAM(spi, i2c, selectCAM) {

}

/**
 * Destructor is currently empty
 */
Payload::~Payload() {
}

/**
 * Initializes interface to payload
 * Initializes camera
 * @return error code
 */
uint8_t Payload::initialize() {
  DEBUG("Payload", "Starting initialization");
  CONSOLE_TX("Payload", "Starting initialization");
  uint8_t result = arduCAM.initCam();
  if (result != ERROR_SUCCESS) {
    DEBUG("Payload", "ArduCAM initialization error: 0x%02x", result);
    CONSOLE_TX("Payload", "ArduCAM initialization error: 0x%02x", result);
    return result;
  }

  DEBUG("Payload", "Initialization complete");
  CONSOLE_TX("Payload", "Initialization complete");
  return ERROR_SUCCESS;
}

/**
 * Requests a frame capture from the camera
 * @return error code
 */
uint8_t Payload::takePicture() {
  DEBUG("Payload", "Taking a photo");
  arduCAM.flushFifo();
  arduCAM.startCapture();

  return ERROR_SUCCESS;
}

/**
 * Gets the frame buffer from the camera and saves it to the SD card
 * @return error code
 */
uint8_t Payload::savePicture() {
  if (!arduCAM.getBit(ARDUCHIP_TRIG, CAP_DONE_MASK)) {
    DEBUG("Payload", "Capture not complete");
    return ERROR_BUSY;
  }

  DEBUG("Payload", "Capture complete, saving to SD card");
  FILE *fp = fopen("/sd/image.csri", "w");
  if (!fp) {
    DEBUG("takePhoto", "Error opening file");
    CONSOLE_TX("takePhoto", "Failed: 0x%02x", ferror(fp));
    return ERROR_OPEN_FAILED;
  }
  CSRI::writeHeaderCSRI(fp, RESOLUTION_VGA, SE, GRBG, 8);

  uint8_t buf[BURST_LENGTH];
  uint32_t bufferLength = 640 * 480; //arduCAM.readFifoLength();
  uint16_t percentUpdate = bufferLength / 100;
  uint32_t currentByte = 0;
  uint16_t bufferIndex = 0;
  DEBUG("takePhoto", "Saving: %luB", bufferLength);
  DEBUG("takePhoto", "Saving: %3d%%", currentByte * 100 / bufferLength);
  while (currentByte <= bufferLength) {
    buf[bufferIndex] = arduCAM.readFifo();
//    arduCAM.readFifo(buf, BURST_LENGTH);
//    if((currentByte % 1024) == 1023){
//      buf[bufferIndex] = arduCAM.readFifo();
//    }
    currentByte += BURST_LENGTH;
    bufferIndex += BURST_LENGTH;
    if (bufferIndex >= BURST_LENGTH) {
      fwrite(buf, 1, BURST_LENGTH, fp);
      bufferIndex = 0;
    }
    if ((currentByte % percentUpdate) == 0) {
      DEBUG("takePhoto", "Saving: %3d%%", currentByte * 100 / bufferLength);
    }
  }
  if (bufferIndex != 0) {
    //Some bytes left over to write
    fwrite(buf, 1, bufferIndex, fp);
    bufferIndex = 0;
  }
  fclose(fp);
  DEBUG("takePhoto", "Finsihed saving image");
  arduCAM.flushFifo();
}
