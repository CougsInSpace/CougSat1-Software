#include "VoltageNode.h"
#include <CISConsole.h>

/**
 * @brief Construct a new Voltage Node::Voltage Node object
 *
 * @param adc connected to the node
 * @param channel connected to the node
 * @param gain to multiply adc voltage by to get node voltage
 */
VoltageNode::VoltageNode(ADC & adc, ADCChannel_t channel, double gain) :
  adc(adc) {
  this->channel = channel;
  this->gain    = gain;
}

/**
 * @brief Get the voltage at the node
 *
 * @return double voltage in volts
 */
double VoltageNode::get() {
  return lastVoltage;
}

/**
 * @brief Update the voltage by reading the ADC's value and doing maths
 * Sets lastVoltage field
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t VoltageNode::update() {
  double              volts = 0.0;
  mbed_error_status_t error = adc.readVoltage(channel, volts);
  if (error) {
    LOGE("VoltageNode", "Failed to read Voltage from ADC: 0x%08X", error);
    return error;
  }
  lastVoltage = volts * gain;
  return error;
}

/**
 * @brief Update the voltage by reading the ADC's value and doing maths
 * Sets lastVoltage field and returns value
 *
 * @param value to read into in volts
 * @return mbed_error_status_t
 */
mbed_error_status_t VoltageNode::updateAndGet(double & value) {
  mbed_error_status_t error = update();
  value                     = get();
  return error;
}
