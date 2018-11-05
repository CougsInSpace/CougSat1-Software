/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file Thermistor.h
 * @author Bradley Davis
 * @date 3 Nov 2018
 * @brief A class for a temperature sensor (thermistor based)
 *
 */

#ifndef _SRC_COMPONENTS_TEMPERATURE_SENSOR_H_
#define _SRC_COMPONENTS_TEMPERATURE_SENSOR_H_

#include "drivers/LTC2499.h"
#include "mbed.h"

class Thermistor {
public:
  Thermistor(LTC2499 &adc, LTC2499Channel_t channel, double voltageFraction270K,
             double voltageFraction350K);
  uint8_t getTemperature(double *data);

private:
  LTC2499 &adc;
  LTC2499Channel_t channel;
  double resistance270K;
  double beta;
};

#endif /* _SRC_COMPONENTS_TEMPERATURE_SENSOR_H_ */