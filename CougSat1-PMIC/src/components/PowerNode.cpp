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
// PowerNode(LTC2499& adc, LTC2499Channel_t channel double shunt, int priority_level, double expectedCurrent); : adc(adc) 
// {
//   this->channel = channel;
//   this->shunt   = shunt;
//   this->pathA   = false;
//   this->pathB   = false;
//   this->priority =  priority_level;
//   this->expectedCurrent = expectedCurrent;

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
void PowerNode::getSwitch(bool * pathA, bool * pathB)
 {
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
  double  value  = 0.0;
  uint8_t result = adc.readVoltage(channel, &value);
  if (result != ERROR_SUCCESS) {
    ERROR("PowerNode", "Failed to read shunt resistor: 0x%02X", result);
    return result;
  }
  DEBUG("PowerNode", "Shunt resistor is %8.6fV", value);
  (*current) = value / shunt;
  return ERROR_SUCCESS;
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

/**
 * @brief Gets the priority level of the node
 * @return uint9_t error code
 * @param value to write to
 * 
 */

uint8_t PowerNode::getPriority(int *priority_value)
{
  *priority_value = this->priority;
  return ERROR_NOT_SUPPORTED;
}

/**
 * @brief Gets expected current for given payload
 * 
 * 
 */
uint8_t PowerNode::getExpectedCurrent(double *current )
{

   return ERROR_NOT_SUPPORTED;
}
