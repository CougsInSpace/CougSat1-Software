#ifndef _LIBRARY_DRIVER_ADC_AD7291_H_
#define _LIBRARY_DRIVER_ADC_AD7291_H_

#include "ADC.h"

#include <mbed.h>

// H = high, F = float, L = low
enum class AD7291Addr_t : uint8_t {
  HH = 0x20 << 1,
  HF = 0x22 << 1,
  HL = 0x23 << 1,
  FH = 0x28 << 1,
  FF = 0x2A << 1,
  FL = 0x2B << 1,
  LH = 0x2C << 1,
  LF = 0x2E << 1,
  LL = 0x2F << 1
};

class AD7291 : public ADC {
public:
  AD7291(const AD7291 &) = delete;
  AD7291 & operator=(const AD7291 &) = delete;

  AD7291(I2C & i2c, AD7291Addr_t addr, double refVoltage = 2.5);
  ~AD7291();

  mbed_error_code_t readRaw(
      ADCChannel_t channel, int32_t & value, bool blocking = true);

private:
  I2C &        i2c;
  AD7291Addr_t addr;

  const uint8_t BIT_DEPTH = 12;
};

#endif /* _LIBRARY_DRIVER_ADC_AD7291_H_ */