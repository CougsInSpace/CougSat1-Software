#ifndef _LIBRARY_COMPONENT_VOLTAGE_NODE_H_
#define _LIBRARY_COMPONENT_VOLTAGE_NODE_H_

#include <ADC/ADC.h>
#include <mbed.h>

class VoltageNode {
public:
  VoltageNode(ADC & adc, ADCChannel_t channel, double gain);

  double              get();
  mbed_error_status_t update();
  mbed_error_status_t updateAndGet(double & value);

private:
  ADC &        adc;
  ADCChannel_t channel;
  double       gain;
  double       lastVoltage;
};

#endif /* _LIBRARY_COMPONENT_VOLTAGE_NODE_H_ */