/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PowerNodeTCA9535.cpp
 * @author Bradley Davis
 * @date 4 Nov 2018
 * @brief A class for a power node consisting of switching, and current
 * monitoring whose switches are connected to a TCA9535 GPIO expander
 *
 */

#include "PowerNodeTCA9535.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

/**
 * @brief Construct a new Power Node TCA9535:: Power Node TCA9535 object
 *
 * @param adc connected to the shunt
 * @param channel connected to the shunt
 * @param shunt resistance
 * @param gpio connected to the switches
 * @param switchA control pin on the GPIO
 * @param switchB control pin on the GPIO
 */
PowerNodeTCA9535::PowerNodeTCA9535(LTC2499 & adc, LTC2499::ADCChannel_t channel,
    double shunt, TCA9535 & gpio, TCA9535::Pin_t switchA,
    TCA9535::Pin_t switchB) :
  PowerNode(adc, channel, shunt),
  gpio(gpio) {
  this->switchA = switchA;
  this->switchB = switchB;
}

/**
 * @brief Disables or enables the switch for either current path
 *
 * @param pathA is connected if true
 * @param pathB is connected if true
 * @return uint8_t error code
 */
uint8_t PowerNodeTCA9535::setSwitch(bool pathA, bool pathB) {
  uint8_t result = gpio.write(switchA, inverted ? !pathA : pathA);
  if (result != ERROR_SUCCESS) {
    return result;
  }
  this->pathA = pathA;

  result = gpio.write(switchB, inverted ? !pathB : pathB);
  if (result != ERROR_SUCCESS) {
    return result;
  }
  this->pathB = pathB;

  return ERROR_SUCCESS;
}