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

typedef enum LTC2499Channel {
  DIFF_0 = 0xA0,
  DIFF_1 = 0xA8,
  DIFF_2 = 0xA1,
  DIFF_3 = 0xA9,
  DIFF_4 = 0xA2,
  DIFF_5 = 0xAA,
  DIFF_6 = 0xA3,
  DIFF_7 = 0xAB,
  DIFF_8 = 0xA4,
  DIFF_9 = 0xAC,
  DIFF_10 = 0xA5,
  DIFF_11 = 0xAD,
  DIFF_12 = 0xA6,
  DIFF_13 = 0xAE,
  DIFF_14 = 0xA7,
  DIFF_15 = 0xAF,
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
} LTC2499Channel_t;

#define LTC2499_CONFIG_CHANNEL ((uint8_t)0xA0)
#define LTC2499_CONFIG_EXT_50_60_1x ((uint8_t)0x80)  // 147ms
#define LTC2499_CONFIG_EXT_50_1x ((uint8_t)0x90)     // 160ms
#define LTC2499_CONFIG_EXT_60_1x ((uint8_t)0xA0)     // 134ms
#define LTC2499_CONFIG_EXT_50_60_2x ((uint8_t)0x88)  // 74ms
#define LTC2499_CONFIG_EXT_50_2x ((uint8_t)0x98)     // 80ms
#define LTC2499_CONFIG_EXT_60_2x ((uint8_t)0xA8)     // 67ms
#define LTC2499_CONFIG_TEMP_50_60_1x ((uint8_t)0xC0) // 147ms
#define LTC2499_CONFIG_TEMP_50_1x ((uint8_t)0xD0)    // 160ms
#define LTC2499_CONFIG_TEMP_60_1x ((uint8_t)0xE0)    // 134ms

#define LTC2499_RAW_MASK_POS (0x01FFFFFF)
#define LTC2499_RAW_MASK_NEG (0xFE000000)

#define LTC2499_OVERRANGE (0x7FFFFFFF)
#define LTC2499_UNDERRANGE (0x80000000)
#define LTC2499_FULL_SCALE (0x01000000)

#define LTC2499_TEMP_SLOPE (1570.0f)

#define LTC2499_CONVERSION_TIMEOUT (200)

class LTC2499 {
public:
  LTC2499(I2C &i2c, uint8_t addr);
  LTC2499(I2C &i2c, uint8_t addr, double refVoltage, double gain);
  uint8_t readVoltage(double *data);
  uint8_t readVoltage(LTC2499Channel_t channel, double *data);
  uint8_t readVoltageSelectNext(double *data, LTC2499Channel_t nextChannel,
                                bool blocking = true);
  uint8_t readNextActiveChannel(bool blocking = true);
  uint8_t addActiveChannel(LTC2499Channel_t channel);
  uint8_t readInternalTemperaure(double *data);
  uint8_t selectChannel(LTC2499Channel_t channel, bool blocking = true);
  void setVRef(double refVoltage, double gain);
  uint8_t setVRef(double refVoltage, double gain, LTC2499Channel_t channel);
  double getRefPin();
  double getVoltage(LTC2499Channel_t channel);

private:
  uint8_t readRaw(int32_t *data, bool blocking = true);
  uint8_t configureChannel(LTC2499Channel_t channel, uint8_t config);

  I2C &i2c;
  uint8_t addr;
  double conversionFactor;
  double vRefVoltage;
  LTC2499Channel_t configuredChannel;

  uint32_t activeChannels;
  double voltages[24];
};

#endif /* _SRC_DRIVERS_LTC2499_H_ */