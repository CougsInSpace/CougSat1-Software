#ifndef _LIBRARY_DRIVER_ADC_AD7689_H_
#define _LIBRARY_DRIVER_ADC_AD7689_H_

#include "ADC.h"

#include <mbed.h>

class AD7689 : public ADC {
public:
  AD7689(const AD7689 &) = delete;
  AD7689 & operator=(const AD7689 &) = delete;

  AD7689(PinName mosi, PinName miso, PinName sclk, PinName cnv,
      double refVoltage = 2.5, double tempSlope = 1000,
      double tempOffset = 0.258);
  ~AD7689();

  mbed_error_status_t readRaw(ADCChannel_t channel, int32_t & value);

  mbed_error_status_t selfTest();

private:
  enum class InputConfig_t : uint8_t {
    BIPOLAR_DIFF  = 0x0,
    BIPOLAR       = 0x2,
    TEMP          = 0x3,
    UNIPOLAR_DIFF = 0x4,
    UNIPOLAR_COM  = 0x6,
    UNIPOLAR_GND  = 0x7
  };

  enum class ReferenceConfig_t : uint8_t {
    INT_2500_TEMP_BUF = 0x0, // Internal 2.5V, temp & buffer enabled
    INT_4096_TEMP_BUF = 0x1, // Internal 4.096V, temp & buffer enabled
    EXT_TEMP          = 0x2, // External, temp enabled, buffer disabled
    EXT_TEMP_BUF      = 0x3, // External, temp & buffer enabled
    EXT               = 0x6, // External, temp & buffer disabled
    EXT_BUF           = 0x7  // External, buffer enabled, temp disabled
  };

  enum class ChannelSequencer_t : uint8_t {
    DISABLED      = 0x0,
    CHANNELS_TEMP = 0x2, // Channels 0 to channel then temp
    CHANNELS      = 0x3  // Channels 0 to channel
  };

  mbed_error_status_t transfer(
      const uint16_t write, uint16_t * read, bool conversionDelay = true);
  mbed_error_status_t transfer(
      const uint16_t write, uint32_t * read, bool conversionDelay = true);
  uint16_t getConfigRegister(bool readback = false);

  SPI spi;

  static const uint16_t DELAY_CNV_US = 6;
  static const uint8_t  BIT_DEPTH    = 16;

  InputConfig_t      inputConfig   = InputConfig_t::UNIPOLAR_COM;
  uint8_t            inputChannel  = 0;
  bool               lowPassFilter = false; // Filter at 1/4 BW
  ReferenceConfig_t  refConfig     = ReferenceConfig_t::INT_2500_TEMP_BUF;
  ChannelSequencer_t channelSeq    = ChannelSequencer_t::DISABLED;
};

#endif /* _LIBRARY_DRIVER_ADC_AD7689_H_ */