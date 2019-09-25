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
    PinName switchOut, bool inverted, uint8_t priority) :
  switchOut(switchOut, inverted ? 1 : 0),
  adc(adc) {
  this->channel  = channel;
  this->shunt    = shunt;
  this->inverted = inverted;
  this->priority = priority;
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
 * @brief Calculates the AggregatePriority value
 *
 * @return aggregate Priority value
 */
double PowerNode::getAggregatePriority()
{
  double AggregatePriority = 0;
  double Measured_Current = 0;

  //1. Get current
  this->getCurrent(Measured_Current);

  //2. Multiply current with priority value
  AggregatePriority = this->priority * Measured_Current;

  return AggregatePriority;
} 


/**
 * @brief Gets the current flowing through the node
 *
 * @param current to read into
 * @return mbed_error_status_t
 */
mbed_error_status_t PowerNode::getCurrent(double & current) {
  // Read ADC to get voltage
  // current = voltage / shunt
  // double  value  = 0.0;
  // mbed_error_status_t result = adc.readVoltage(channel, value);
  // if (!result != ERROR_SUCCESS) {
  //   ERROR("PowerNode", "Failed to read shunt resistor: 0x%02X", result);
  //   return result;
  // }
  // DEBUG("PowerNode", "Shunt resistor is %8.6fV", value);
  // (*current) = value / shunt;
  return MBED_ERROR_UNSUPPORTED;
}

/**
 * @brief Set the state of the switch
 *
 * @param on will close the switch if true, open if false
 */
void PowerNode::setSwitch(bool on) {
  switchOut = inverted ^ on;
}