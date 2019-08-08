/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PowerNodeDirect.cpp
 * @author Bradley Davis
 * @date 4 Nov 2018
 * @brief A class for a power node consisting of switching, and current
 * monitoring whose switches are connected direct to the processor
 *
 */

#include "PowerNodeDirect.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"
#include "drivers/LTC2499.h"

/**
 * @brief Construct a new Power Node Direct:: Power Node Direct object
 *
 * @param adc connected to the shunt
 * @param channel connected to the shunt
 * @param shunt resistance
 * @param pinSwitchA control pin
 * @param pinSwitchB control pin
 */
PowerNodeDirect::PowerNodeDirect(LTC2499 & adc, LTC2499::ADCChannel_t channel, double shunt,
                PinName pinSwitchA, PinName pinSwitchB) :
  PowerNode(adc, channel, shunt),
  switchA(switchA), switchB(switchB) 
{
  setSwitch(false, false);
}

/**
 * @brief Disables or enables the switch for either current path
 *
 * @param pathA is connected if true
 * @param pathB is connected if true
 * @return uint8_t error code
 */
uint8_t PowerNodeDirect::setSwitch(bool pathA, bool pathB) {
  this->pathA = pathA;
  this->pathB = pathB;
  DEBUG("PowerNode", "Setting direct switch");
  
  switchA = 1; //inverted ? !pathA : pathA;
  switchB = 1; //inverted ? !pathB : pathB;
  return ERROR_SUCCESS;
}