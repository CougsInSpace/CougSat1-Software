#ifndef _LIBRARY_DRIVER_SYNTH_ADF4360_H_
#define _LIBRARY_DRIVER_SYNTH_ADF4360_H_

#include <mbed.h>

#include "Synth.h"

struct ADF4360Spec {
  uint32_t vcoMinFreq;
  uint32_t vcoMaxFreq;
  uint32_t countersMaxFreq;
  uint8_t  maxPrescaler;
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

class ADF4360 : public Synth {
public:
  ADF4360(const ADF4360 &) = delete;
  ADF4360 & operator=(const ADF4360 &) = delete;

  ADF4360(SPI & spi, PinName cs, const uint8_t variant, const uint32_t ref,
      const uint16_t minCounterR = 1);
  ~ADF4360();

  mbed_error_status_t setFrequency(uint32_t freq);
  mbed_error_status_t setEnable(bool enable);

private:
  enum class PowerDown_t : uint8_t { ON = 0, ASYNC_OFF = 1, SYNC_OFF = 3 };

  enum class OutputPower_t : uint8_t {
    MA_3_5  = 0, // 3.5mA (50ohms = -14dBm)
    MA_5_0  = 1,
    MA_7_5  = 2,
    MA_11_0 = 3
  };

  enum class CorePower_t : uint8_t {
    MA_5  = 0, // 5mA
    MA_10 = 1,
    MA_15 = 2,
    MA_20 = 3
  };

  enum class CurrentSetting_t : uint8_t {
    // For RSET = 4.7kiloohms
    MA_0_31 = 0,
    MA_0_62 = 1,
    MA_0_93 = 2,
    MA_1_25 = 3,
    MA_1_56 = 4,
    MA_1_87 = 5,
    MA_2_18 = 6,
    MA_2_50 = 7
  };

  enum class MuxOutput_t : uint8_t {
    TRI_STATE       = 0,
    LOCK_DETECT     = 1,
    N_DIVIDER       = 2,
    DVDD            = 3,
    R_DIVIDER       = 4,
    LOCK_DETECT_OD  = 5,
    SERIAL_DATA_OUT = 6,
    GND             = 7
  };

  enum class BandSelect_t : uint8_t { ONE = 0, TWO = 1, FOUR = 2, EIGHT = 3 };

  enum AntiBacklash_t : uint8_t { NS_1_3 = 1, NS_3_0 = 0, NS_6_0 = 2 };

  enum class Register_t : uint8_t {
    CONTROL   = 0x00,
    COUNTER_R = 0x01,
    COUNTER_N = 0x02
  };

  void tuneRCounter(uint16_t & counter);

  mbed_error_status_t write(Register_t reg);

  SPI &         spi;
  DigitalOut    cs;
  const uint8_t variant;

  uint8_t          prescaler               = 1;
  PowerDown_t      powerDown               = PowerDown_t::ON;
  CurrentSetting_t currentSetting1         = CurrentSetting_t::MA_2_50;
  CurrentSetting_t currentSetting2         = CurrentSetting_t::MA_2_50;
  OutputPower_t    outputPower             = OutputPower_t::MA_11_0;
  bool             muteTillLock            = true;
  bool             cpGainUse2              = false; // true=current 2, false=1
  bool             cpOutputTriState        = false;
  bool             phaseDetectPositive     = true;
  MuxOutput_t      muxOutput               = MuxOutput_t::DVDD;
  bool             countersReset           = false;
  CorePower_t      corePower               = CorePower_t::MA_5;
  bool             divide2Select           = false;
  bool             divide2                 = false;
  uint16_t         counterB                = 0;
  uint8_t          counterA                = 0;
  BandSelect_t     bandSelect              = BandSelect_t::ONE;
  bool             lockDetectHighPrecision = false; // true=5 cycles, false=3
  AntiBacklash_t   antiBacklash            = AntiBacklash_t::NS_3_0;
  uint16_t         counterR                = 0;

  uint16_t minCounterR = 1;

  static const uint32_t MAX_FREQ_PFD = 8000000;
};

#endif /* _LIBRARY_DRIVER_SYNTH_ADF4360_H_ */