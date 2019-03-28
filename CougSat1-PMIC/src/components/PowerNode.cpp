/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PowerNode.cpp
 * @author Bradley Davis
 * @date 3 Nov 2018
 * @brief A class for a power node consisting of switching, and current
 * monitoring
 *
 */

#include "PowerNode.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

/**
 * @brief Construct a new Power Node::Power Node object
 *
 * @param adc connected to the shunt
 * @param channel connected to the shunt
 * @param shunt resistance
 */
PowerNode::PowerNode(
    LTC2499 & adc, LTC2499::ADCChannel_t channel, double shunt) :
  adc(adc) {
  this->channel  = channel;
  this->shunt    = shunt;
  this->pathA    = false;
  this->pathB    = false;
  this->inverted = true;
  adc.addActiveChannel(channel);
}

/**
 * @brief Gets the current state of the node's path switched
 *
 * @param pathA is connected if true
 * @param pathB is connected if true
 */
void PowerNode::getSwitch(bool * pathA, bool * pathB) {
  *(pathA) = this->pathA;
  *(pathB) = this->pathB;
}

/**
 * @brief Gets the current flowing through the node
 *
 * @param current to read into
 * @return uint8_t error code
 */
uint8_t PowerNode::getCurrent(double * current) {
  // Read ADC to get voltage
  // current = voltage / shunt
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Sets the switches of the node (not supported)
 *
 * @param pathA is connected if true
 * @param pathB is connected if true
 * @return uint8_t error code
 */
uint8_t PowerNode::setSwitch(bool pathA, bool pathB) {
  ERROR("PowerNode", "Set switch improperly called on the base class");
  return ERROR_NOT_SUPPORTED;
}