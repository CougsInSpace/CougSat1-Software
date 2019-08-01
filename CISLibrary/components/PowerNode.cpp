#include "PowerNode.h"

/**
 * @brief Construct a new Power Node::Power Node object
 *
 * @param adc connected to the shunt
 * @param channel connected to the shunt
 * @param shunt resistance in ohms
 * @param switchOut pin for enabling
 * @param inverted if set will invert the switchOut
 */
PowerNode::PowerNode(ADC & adc, ADCChannel_t channel, double shunt,
    PinName switchOut, bool inverted) :
  switchOut(switchOut, inverted ? 1 : 0),
  adc(adc) {
  this->channel  = channel;
  this->shunt    = shunt;
  this->inverted = inverted;
}

/**
 * @brief Get the current state of the switch
 *
 * @return true if closed or no switch connected
 * @return false if otherwise
 */
bool PowerNode::getSwitch() {
  if (switchOut.is_connected())
    return switchOut.read() ^ inverted;
  return true;
}

/**
 * @brief Gets the current flowing through the node
 *
 * @param current to read into
 * @return CISResult_t error code
 */
CISResult_t PowerNode::getCurrent(double & current) {
  // Read ADC to get voltage
  // current = voltage / shunt
  // double  value  = 0.0;
  // CISResult_t result = adc.readVoltage(channel, value);
  // if (!result != ERROR_SUCCESS) {
  //   ERROR("PowerNode", "Failed to read shunt resistor: 0x%02X", result);
  //   return result;
  // }
  // DEBUG("PowerNode", "Shunt resistor is %8.6fV", value);
  // (*current) = value / shunt;
  return {ERROR_NOT_SUPPORTED, ""};
}

/**
 * @brief Set the state of the switch
 *
 * @param on will close the switch if true, open if false
 */
void PowerNode::setSwitch(bool on) {
  switchOut = inverted ^ on;
}