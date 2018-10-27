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

typedef enum ADCChannel {
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
  SINGLE_15 = 0xBF,
  INT_TEMP = 0x80
} ADCChannel_t;

#define ADC_CONFIG_CHANNEL ((uint8_t)0xA0)
#define ADC_CONFIG_EXT_50_60_1x ((uint8_t)0x80)
#define ADC_CONFIG_EXT_50_60_2x ((uint8_t)0x88)
#define ADC_CONFIG_TEMP_50_60_1x ((uint8_t)0xC0)

#define ADC_RAW_MASK_POS (0x01FFFFFF)
#define ADC_RAW_MASK_NEG (0xFE000000)

#define ADC_OVERRANGE (0x7FFFFFFF)
#define ADC_UNDERRANGE (0x80000000)
#define ADC_FULL_SCALE (0x01000000)

#define ADC_ZERO_KELVIN (-273.15f)
#define ADC_TEMP_SLOPE (1570.0f)

#define ADC_CONVERSION_TIMEOUT (200)

class LTC2499 {
public:
  LTC2499(I2C &i2c, uint8_t addr);
  LTC2499(I2C &i2c, uint8_t addr, double refVoltage, double gain);
  uint8_t readVoltage(double *data);
  uint8_t readVoltage(ADCChannel_t channel, double *data);
  uint8_t readInternalTemperaure(double *data);
  uint8_t selectChannel(ADCChannel_t channel, bool blocking = true);
  void setVRef(double refVoltage, double gain);
  uint8_t setVRef(double refVoltage, ADCChannel_t channel);

private:
  uint8_t readRaw(int32_t *data, bool blocking = true);
  uint8_t configureChannel(ADCChannel_t channel, uint8_t config);

  I2C &i2c;
  uint8_t addr;
  double conversionFactor;
  double refVoltage;
  ADCChannel_t configuredChannel;
};

#endif /* _SRC_DRIVERS_LTC2499_H_ */