#include "CurrentNode.h"
#include <CISConsole.h>

/**
 * @brief Construct a new Current Node::Current Node object
 *
 * @param adc connected to the shunt
 * @param channel connected to the shunt
 * @param gain to multiply adc voltage by to get amps
 * @param switchOut pin for enabling
 * @param inverted if set will invert the switchOut
 * @param rank of the node: Largest (rank * current) is first to be shed
 */
CurrentNode::CurrentNode(ADC & adc, ADCChannel_t channel, double gain,
    PinName switchOut, bool inverted, uint8_t rank) :
  switchOut(switchOut, inverted ? 1 : 0),
  adc(adc) {
  this->channel  = channel;
  this->gain     = gain;
  this->inverted = inverted;
  this->rank     = rank;
}

/**
 * @brief Get the current state of the switch
 *
 * @return true if closed or no switch connected
 * @return false if otherwise
 */
bool CurrentNode::getSwitch() {
  if (switchOut.is_connected())
    return switchOut.read() ^ inverted;
  return true;
}

/**
 * @brief Get the aggregate rank (current * rank)
 *
 * @return double (current * rank)
 */
double CurrentNode::getAggregateRank() {
  return (getCurrent() * rank);
}

/**
 * @brief Get the current flowing through the node
 *
 * @return double current in amps
 */
double CurrentNode::getCurrent() {
  return lastCurrent;
}

/**
 * @brief Update the current by reading the ADC's value and doing maths
 * Sets lastCurrent field
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t CurrentNode::updateCurrent() {
  double              volts = 0.0;
  mbed_error_status_t error = adc.readVoltage(channel, volts);
  if (error) {
    ERROR("CurrentNode", "Failed to read Voltage from ADC: 0x%08X", error);
    return error;
  }
  lastCurrent = volts * gain;
  return error;
}

/**
 * @brief Set the state of the switch
 *
 * @param on will close the switch if true, open if false
 */
void CurrentNode::setSwitch(bool on) {
  switchOut = inverted ^ on;
}