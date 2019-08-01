#ifndef _LIBRARY_COMPONENT_POWER_NODE_H_
#define _LIBRARY_COMPONENT_POWER_NODE_H_

#include <ADC/ADC.h>
#include <mbed.h>

class PowerNode {
public:
  PowerNode(ADC & adc, ADCChannel_t channel, double shunt,
      PinName switchOut = NC, bool inverted = false);

  CISResult_t getCurrent(double & current);

  bool getSwitch();

  void setSwitch(bool on);

private:
  DigitalOut switchOut;
  bool       inverted;

  ADC &        adc;
  ADCChannel_t channel;
  double       shunt;
};

#endif /* _LIBRARY_COMPONENT_POWER_NODE_H_ */