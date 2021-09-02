#ifndef _LIBRARY_DRIVER_ADC_AD7291_H_
#define _LIBRARY_DRIVER_ADC_AD7291_H_

#include "ADC.h"

#include <mbed.h>

// H = high, F = float, L = low
// AS1, AS0
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

  AD7291(I2C & i2c, AD7291Addr_t addr, double refVoltage = 2.5,
      double tempSlope = 0.25, double tempOffset = 0.0);
  ~AD7291();

  mbed_error_status_t readRaw(ADCChannel_t channel, int32_t & value);

  mbed_error_status_t selfTest();

  mbed_error_status_t reset();

private:
  enum class Register_t : uint8_t {
    CONTROL         = 0x00,
    RESULT_VOLTAGE  = 0x01,
    RESULT_TEMP     = 0x02,
    RESULT_TEMP_AVG = 0x03,
    DATA_HIGH_CH0   = 0x04, // Ch(x) = Ch0 + 3 * x
    DATA_LOW_CH0    = 0x05, // Ch(x) = Ch0 + 3 * x
    DATA_HYS_CH0    = 0x06, // Ch(x) = Ch0 + 3 * x
    DATA_HIGH_TEMP  = 0x1C,
    DATA_LOW_TEMP   = 0x1D,
    DATA_HYS_TEMP   = 0x1E,
    ALERT_A         = 0x1F,
    ALERT_B         = 0x20
  };

  mbed_error_status_t read(
      Register_t reg, uint16_t & value, uint8_t registerOffset = 0);
  mbed_error_status_t write(
      Register_t reg, uint16_t value, uint8_t registerOffset = 0);
  mbed_error_status_t writeControlRegister();

  I2C &        i2c;
  AD7291Addr_t addr;

  static const uint8_t BIT_DEPTH = 12;

  uint8_t channels;
  bool    tempSense         = true;
  bool    noiseDelayed      = true;
  bool    externalReference = false;
  bool    alertActiveLow    = false;
  bool    clearAlert        = false;
  bool    autocycle         = false;
};

#endif /* _LIBRARY_DRIVER_ADC_AD7291_H_ */