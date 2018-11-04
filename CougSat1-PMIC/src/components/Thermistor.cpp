/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file Thermistor.cpp
 * @author Bradley Davis
 * @date 3 Nov 2018
 * @brief A class for a temperature sensor (thermistor based)
 *
 */

#include "Thermistor.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

/**
 * @brief Construct a new Temperature Sensor::Temperature Sensor object
 * Voltage fraction is the voltage of the thermistor / the thermistor's source
 * voltage
 *
 * @param voltageFraction25C proportion of thermistor at 25°C
 * @param voltageFraction85C proportion of thermistor at 25°C
 */
Thermistor::Thermistor(double voltageFraction25C,
                                     double voltageFraction85C) {
  // resistance = 1/fraction - 1
  // beta = ln(R25/R85)/(1/(25C in kelvin)-1/(85c in kelvin))
}

uint8_t Thermistor::getTemperature(double *data) {
  // Read voltage fraction of thermistor (Vtherm/Vsource)
  // resistance = (1/fraction -1)/resistance25C
  // temp = 1/(1/(25c in kelvin)+ln(resistance)/beta)
}