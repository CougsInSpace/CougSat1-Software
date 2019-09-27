#ifndef _LIBRARY_COMPONENT_CURRENT_NODE_H_
#define _LIBRARY_COMPONENT_CURRENT_NODE_H_

#include <ADC/ADC.h>
#include <mbed.h>

class CurrentNode {
public:
  CurrentNode(ADC & adc, ADCChannel_t channel, double gain,
      PinName switchOut = NC, bool inverted = false, uint8_t rank = 0);

  double              getCurrent();
  mbed_error_status_t updateCurrent();

  mbed_error_status_t getAggregateRank(double & aggregateRank);

  bool getSwitch();

  void setSwitch(bool on);

private:
  DigitalOut switchOut;
  bool       inverted;

  ADC &        adc;
  ADCChannel_t channel;
  double       gain;
  double       lastCurrent;

  uint8_t rank;
};

#endif /* _LIBRARY_COMPONENT_POWER_NODE_H_ */