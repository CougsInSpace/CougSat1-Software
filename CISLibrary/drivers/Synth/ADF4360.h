#ifndef _LIBRARY_DRIVER_SYNTH_ADF4360_H_
#define _LIBRARY_DRIVER_SYNTH_ADF4360_H_

#include <mbed.h>

struct ADF4360Spec {
  uint32_t vcoMinFreq;
  uint32_t vcoMaxFreq;
  uint32_t countersMaxFreq;
  uint8_t  maxPrescaller;
};

// clang-format off
static const ADF4360Spec ADF4360Variants[] = {
    {2400000000, 2725000000, 300000000, 32},
    {2050000000, 2450000000, 300000000, 32},
    {1850000000, 2150000000, 300000000, 32},
    {1600000000, 1950000000, 300000000, 32},
    {1450000000, 1750000000, 300000000, 32},
    {1200000000, 1400000000, 300000000, 32},
    {1050000000, 1250000000, 300000000, 32},
    { 350000000, 1800000000, 300000000, 16},
    {  65000000,  400000000, 400000000,  1},
    {  65000000,  400000000, 400000000,  1}
};
// clang-format on

class ADF4360 {
public:
  ADF4360(const ADF4360 &) = delete;
  ADF4360 & operator=(const ADF4360 &) = delete;

  ADF4360(SPI & spi, PinName cs, uint8_t variant);
  ~ADF4360();

  mbed_error_status_t setFrequency(uint32_t freq);
  mbed_error_status_t setEnable(bool enable);

private:
  enum class Register_t : uint8_t {
    CONTROL   = 0x00,
    COUNTER_R = 0x01,
    COUNTER_N = 0x02
  };

  mbed_error_status_t tuneRCounter(uint16_t counter);

  mbed_error_status_t write(Register_t reg, uint32_t value);

  SPI &      spi;
  DigitalOut cs;
  uint8_t    variant;
};

#endif /* _LIBRARY_DRIVER_SYNTH_ADF4360_H_ */