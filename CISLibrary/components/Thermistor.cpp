#include "Thermistor.h"

/**
 * @brief Construct a new Thermistor::Thermistor object
 * Voltage fraction is the voltage of the thermistor / the thermistor's source
 * voltage
 *
 * @param voltageFraction25C proportion of thermistor at 270K
 * @param voltageFraction85C proportion of thermistor at 350K
 */
Thermistor::Thermistor(ADC & adc, ADCChannel_t channel,
    double voltageFraction270K, double voltageFraction350K) :
  adc(adc),
  channel(channel) {
  // resistance = 1/fraction - 1
  // beta = ln(R270K/R350K)/(1/(270K)-1/(350K))
}

/**
 * @brief Get the temperature of the thermistor
 *
 * @param data to read into
 * @param blocking will wait for data if set
 * @return uint8_t error code
 */
mbed_error_code_t Thermistor::getTemperature(double & data, bool blocking) {
  // Read voltage fraction of thermistor (Vtherm/Vsource)
  // resistance = (1/fraction - 1)/resistance270K
  // temp = 1/(1/(270K)+ln(resistance)/beta)
  return mbed_error_code_t::MBED_ERROR_UNSUPPORTED;
}