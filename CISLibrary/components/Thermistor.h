#ifndef _LIBRARY_COMPONENT_THERMISTOR_H_
#define _LIBRARY_COMPONENT_THERMISTOR_H_

#include <ADC/ADC.h>
#include <mbed.h>

class Thermistor {
public:
  Thermistor(ADC & adc, ADCChannel_t channel, double voltageFraction270K,
      double voltageFraction350K);

  mbed_error_status_t getTemperature(double & data, bool blocking = true);

private:
  ADC & adc;

  ADCChannel_t channel;

  double resistance270K;
  double beta;
};

#endif /* _LIBRARY_COMPONENT_THERMISTOR_H_ */