/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file TCA9535.h
 * @author Bradley Davis
 * @date 3 Nov 2018
 * @brief Driver to talk to a TCA9535 GPIO Expander
 *
 */

#ifndef _SRC_DRIVERS_TCA9535_H_
#define _SRC_DRIVERS_TCA9535_H_

#include "mbed.h"

class TCA9535 {
public:
  typedef enum Pin {
    P00 = 0x00,
    P01 = 0x01,
    P02 = 0x02,
    P03 = 0x03,
    P04 = 0x04,
    P05 = 0x05,
    P06 = 0x06,
    P07 = 0x07,
    P10 = 0x10,
    P11 = 0x11,
    P12 = 0x12,
    P13 = 0x13,
    P14 = 0x14,
    P15 = 0x15,
    P16 = 0x16,
    P17 = 0x17
  } Pin_t;

  typedef enum Register {
    INPUT    = 0x00,
    OUTPUT   = 0x02,
    POLARITY = 0x04,
    CONFIG   = 0x06
  } Register_t;

  TCA9535(I2C & i2c, uint8_t addr);

  uint8_t configurePin(
      TCA9535::Pin_t pin, bool input, bool polarityInversion, bool value);
  uint8_t configureAll(bool input, bool polarityInversion, bool value);

  uint8_t dumpRegisters();

  uint8_t write(TCA9535::Pin_t pin, bool value);
  uint8_t read(TCA9535::Pin_t pin, bool * value);

private:
  uint8_t writeRegister(
      TCA9535::Register_t reg, TCA9535::Pin_t pin, uint8_t value);
  uint8_t readRegister(
      TCA9535::Register_t reg, TCA9535::Pin_t pin, uint8_t * value);

  I2C &   i2c;
  uint8_t addr;
  uint8_t outputBank0;
  uint8_t outputBank1;
};

#endif /* _SRC_DRIVERS_TCA9535_H_ */