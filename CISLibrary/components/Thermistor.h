#ifndef _LIBRARY_COMPONENT_THERMISTOR_H_
#define _LIBRARY_COMPONENT_THERMISTOR_H_

#include <ADC/ADC.h>
#include <mbed.h>

class Thermistor {
public:
  Thermistor(ADC & adc, ADCChannel_t channel, double voltageFraction300K,
      double voltageFraction340K);

  mbed_error_status_t get(double & temp);

private:
  ADC & adc;

  ADCChannel_t channel;

  double resistance300K;
  double beta;
};

class InternalTemp {
public:
  InternalTemp();

  mbed_error_status_t get(double & temp);

private:
  AnalogIn adc;
  AnalogIn ref;
};

#endif /* _LIBRARY_COMPONENT_THERMISTOR_H_ */