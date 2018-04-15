/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file LTC2499.h
 * @author Bradley Davis
 * @date 13 Apr 2018
 * @brief Driver to talk to a LTC2499 ADC
 *
 */

#ifndef _SRC_DRIVERS_LTC2499_H_
#define _SRC_DRIVERS_LTC2499_H_

#include "mbed.h"

typedef enum ADCChannel_t {
  DIFF_0 = 0xA0,
  DIFF_2 = 0xA1,
  DIFF_4 = 0xA2,
  DIFF_6 = 0xA3,
  DIFF_8 = 0xA4,
  DIFF_10 = 0xA5,
  DIFF_12 = 0xA6,
  DIFF_14 = 0xA7,
  SINGLE_0 = 0xB0,
  SINGLE_1 = 0xB8,
  SINGLE_2 = 0xB1,
  SINGLE_3 = 0xB9,
  SINGLE_4 = 0xB2,
  SINGLE_5 = 0xBA,
  SINGLE_6 = 0xB3,
  SINGLE_7 = 0xBB,
  SINGLE_8 = 0xB4,
  SINGLE_9 = 0xBC,
  SINGLE_10 = 0xB5,
  SINGLE_11 = 0xBD,
  SINGLE_12 = 0xB6,
  SINGLE_13 = 0xBE,
  SINGLE_14 = 0xB7,
  SINGLE_15 = 0xBF
} ADCChannel_t;

#define ADC_CONFIG_EXT_50_60_1x 0x80
#define ADC_CONFIG_EXT_50_60_2x 0x88
#define ADC_CONFIG_TEMP_50_60_1x 0xC0

#define ADC_RAW_MASK 0x01FFFFFF

#define ADC_OVERRANGE 0x7FFFFFFF
#define ADC_UNDERRANGE 0x80000000
#define ADC_FULL_SCALE 0x01000000

class LTC2499 {
public:
  LTC2499(I2C &i2c, uint8_t addr);
  float readVoltage(ADCChannel_t channel);
  uint8_t setVRef(float refVoltage);
  uint8_t setVRef(float refVoltage, ADCChannel_t channel);

private:
  int32_t readRaw(ADCChannel_t channel);

  I2C &i2c;
  uint8_t addr;
  float conversionFactor = 0;
};

#endif /* _SRC_DRIVERS_LTC2499_H_ */