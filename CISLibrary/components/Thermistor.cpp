#include "Thermistor.h"

#include <CISConsole.h>
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
 * @return mbed_error_status_t
 */
mbed_error_status_t Thermistor::get(double & temp) {
  int32_t             raw   = 0;
  mbed_error_status_t error = adc.readRaw(channel, raw);
  if (raw == 0 || raw == adc.MAX_RAW) {
    LOGE("Thermistor", "Read unexpected value value: %9ld", raw);
    return MBED_ERROR_INVALID_DATA_DETECTED;
  }
  double value = (double)raw * adc.CONVERSION_FLOAT;

  // Resistance from voltage divider formula
  double resistance = (1.0 / value - 1.0) / resistance300K;
  // resistance(T) = resistance300K * exp(-beta * (1/300 - 1/T))
  temp = 1.0 / (1.0 / 300.0 + log(resistance) / beta);
  return error;
}

/**
 * @brief Construct a new Internal Temperature object
 *
 */
InternalTemp::InternalTemp() : adc(ADC_TEMP), ref(ADC_VREF) {}

/**
 * @brief Get the temperature of the internal temperature sensor
 *
 * @param data to read into in Kelvin
 * @return mbed_error_status_t
 */
mbed_error_status_t InternalTemp::get(double & temp) {
  const uint16_t TS_CAL1 = *(uint16_t *)(0x1FFF75A8); // Raw 12b value at 30C
  const uint16_t TS_CAL2 = *(uint16_t *)(0x1FFF75CA); // Raw 12b value at 110C
  const uint16_t VREFINT = *(uint16_t *)(0x1FFF75AA); // Raw 12b value at 30C
#ifdef TARGET_STM32H7
  // 16b range
  const double cal303k = (double)TS_CAL1 * (1.0 / (double)0xFFFF);
  const double cal383k = (double)TS_CAL2 * (1.0 / (double)0xFFFF);
  const double calRef  = (double)VREFINT * (1.0 / (double)0xFFFF);
#else
  // 12b range
  const double cal303k = (double)TS_CAL1 * (1.0 / (double)0xFFF);
  const double cal383k = (double)TS_CAL2 * (1.0 / (double)0xFFF);
  const double calRef  = (double)VREFINT * (1.0 / (double)0xFFF);
#endif

  double vref = (double)ref.read();
  if (vref == 0.0 || vref == 1.0) {
    LOGE("IntTemp", "Read unexpected vref value: %9.5f", vref);
    return MBED_ERROR_INVALID_DATA_DETECTED;
  }

  double value = (double)adc.read();
  if (value == 0.0 || value == 1.0) {
    LOGE("IntTemp", "Read unexpected temp value: %9.5f", value);
    return MBED_ERROR_INVALID_DATA_DETECTED;
  }

  // Calibration performed at VDDA = VREF+ = 3.0V, compensate
  // value@3.0 = value@vcc * ref@3.0 / ref@vcc
  value = value * calRef / vref;

  // Temperature varies linearly, interpolate
  temp = (value - cal303k) / (cal383k - cal303k) * (383.15 - 303.15) + 303.15;
  return MBED_SUCCESS;
}
