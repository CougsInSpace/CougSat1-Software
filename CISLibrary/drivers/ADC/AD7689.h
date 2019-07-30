#ifndef _LIBRARY_DRIVER_ADC_AD7689_H_
#define _LIBRARY_DRIVER_ADC_AD7689_H_

#include "ADC.h"
#include "CISError.h"

#include <mbed.h>

class AD7689 : public ADC {
public:
  AD7689(const AD7689 &) = delete;
  AD7689 & operator=(const AD7689 &) = delete;

  AD7689(SPI & spi, PinName cnv, double refVoltage = 2.5);
  ~AD7689();

  CISResult_t readRaw(ADCChannel_t channel, int32_t & value, bool blocking = true);

private:
  SPI &      spi;
  DigitalOut cnv;

  const uint8_t BIT_DEPTH = 16;
};

#endif /* _LIBRARY_DRIVER_ADC_AD7689_H_ */