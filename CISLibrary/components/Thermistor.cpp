#include "Thermistor.h"

#include <cmath>

/**
 * @brief Construct a new Thermistor::Thermistor object
 * Voltage fraction is the voltage of the thermistor / the thermistor's source
 * voltage
 *
 * @param voltageFraction25C proportion of thermistor at 300K
 * @param voltageFraction85C proportion of thermistor at 340K
 */
Thermistor::Thermistor(ADC & adc, ADCChannel_t channel,
    double voltageFraction300K, double voltageFraction340K) :
  adc(adc),
  channel(channel) {
  // Voltage to resistance in the voltage divider
  resistance300K        = 1.0 / voltageFraction300K - 1.0;
  double resistance340K = 1.0 / voltageFraction340K - 1.0;
  // resistance(T) = resistance300K * exp(-beta * (1/300 - 1/T))
  beta = log(resistance300K / resistance340K) / (1.0 / 300.0 - 1.0 / 340.0);
}

/**
 * @brief Get the temperature of the thermistor
 *
 * @param data to read into in Kelvin
 * @return uint8_t error code
 */
mbed_error_status_t Thermistor::getTemperature(double & temp) {
  double              value = 0.0;
  mbed_error_status_t error = adc.readVoltage(channel, value);
  value                     = value / adc.getReferenceVoltage();
  // Resistance from voltage divider formula
  double resistance = (1.0 / value - 1.0) / resistance300K;
  // resistance(T) = resistance300K * exp(-beta * (1/300 - 1/T))
  temp = 1.0 / (1.0 / 300.0 + log(resistance) / beta);
  return error;
}