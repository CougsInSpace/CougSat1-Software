/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file ArduCAM.h
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief communicates between IHU and ArduCAM mini module
 *
 * Based off source code provided by ArduCAM
 *
 * The ArduCAM mini module has two devices
 *    -Camera sensor: Configured through I2C
 *    -ArduChip:      Communicated over SPI
 *
 * The ArduChip does the camera parallel interface to grab a frame and load it
 * into its frame buffer. The frame buffer is read through the ArduChip
 */

#ifndef SRC_DRIVERS_ARDUCAM_H_
#define SRC_DRIVERS_ARDUCAM_H_

#include <mbed.h>
#include "configurations/CameraSensor.h"

#define ARDUCHIP_TIM          0x03  //Timming control
#define VSYNC_LEVEL_MASK      0x02  //0 = High active ,     1 = Low active

#define ARDUCHIP_TRIG         0x41  //Trigger source
#define CAP_DONE_MASK         0x08

#define ARDUCHIP_FIFO         0x04  //FIFO and I2C control
#define FIFO_CLEAR_MASK       0x01
#define FIFO_START_MASK       0x02
#define FIFO_RDPTR_RST_MASK   0x10
#define FIFO_WRPTR_RST_MASK   0x20

#define BURST_FIFO_READ       0x3C  //Burst FIFO read operation
#define SINGLE_FIFO_READ      0x3D  //Single FIFO read operation

#define FIFO_SIZE0            0x42  //Camera write FIFO size[7:0]
#define FIFO_SIZE1            0x43  //Camera write FIFO size[15:8]
#define FIFO_SIZE2            0x44  //Camera write FIFO size[18:16]

#define ARDUCAM_VER_ADDR      0x40
#define ARDUCHIP_TEST1        0x00  //TEST register
#define ARDUCHIP_TEST_VAL     0x55

#define ARDUCAM_2592x1944 0

class ArduCAM {
  public:
    ArduCAM(SPI &spi, I2C &i2c, PinName pinSelect);
    uint8_t initCam();
    void startCapture();
    void flushFifo();
    uint8_t readFifo();
    uint8_t readFifo(uint8_t *buf, uint8_t burstLength);
    uint32_t readFifoLength();
    uint8_t setResolution(uint8_t size);
    uint8_t setFormat(uint8_t format);
    uint8_t getBit(uint8_t addr, uint8_t bit);

  private:
    uint8_t setFifoBurst(uint8_t);
    uint8_t readReg(uint16_t addr, uint8_t *data);
    uint8_t readReg16(uint16_t addr, uint16_t *data);
    uint8_t writeReg(uint16_t addr, uint8_t data);
    uint8_t writeReg16(uint16_t addr, uint16_t data);
    uint8_t writeRegList(const struct CameraReg *regList);
    void setBit(uint8_t addr, uint8_t bit);
    void clearBit(uint8_t addr, uint8_t bit);
    uint8_t readArduCAM(uint8_t addr);
    void writeArduCAM(uint8_t addr, uint8_t data);

    SPI &spi;
    I2C &i2c;
    DigitalOut select;
};

#endif /* SRC_DRIVERS_ARDUCAM_H_ */
