#include "ADF4360.h"

#include <CISConsole.h>

/**
 * @brief Construct a new ADF4360::ADF4360 object
 *
 * @param spi connected to the synth
 * @param cs connected to the synth
 * @param variant of the ADF4360 [0, 9]
 * @param ref clock in Hertz
 * @param minCounterR to divide ref clock by (defines smallest frequency
 * adjustment)
 */
ADF4360::ADF4360(SPI & spi, const PinName cs, const uint8_t variant,
    const uint32_t ref, const uint16_t minCounterR) :
  Synth(ref),
  spi(spi), cs(cs, 1), variant(variant), minCounterR(minCounterR) {
  MBED_ASSERT(variant <= 9);
}

/**
 * @brief Destroy the ADF4360::ADF4360 object
 *
 */
ADF4360::~ADF4360() {}

/**
 * @brief Set the frequency outputted from the synth
 *
 * @param freq in Hertz
 * @return mbed_error_status_t
 */
mbed_error_status_t ADF4360::setFrequency(uint32_t freq) {
  if (freq > ADF4360Variants[variant].vcoMaxFreq)
    return MBED_ERROR_INVALID_ARGUMENT;
  if (freq < ADF4360Variants[variant].vcoMinFreq)
    return MBED_ERROR_INVALID_ARGUMENT;

  uint32_t frequencyPFD;
  counterR = max(minCounterR, (uint16_t)1) - 1;
  if (variant >= 8) {
    prescaler = 1;
    counterA  = 0; // A counter does not exist
    tuneRCounter(counterR);
    frequencyPFD = ref / counterR;
    counterB     = (uint16_t)((float)freq / frequencyPFD + 0.5);
  } else {
    // Increase prescaler until the divided frequency is less than
    // countersMaxFreq
    prescaler = 8;
    while (prescaler < ADF4360Variants[variant].maxPrescaler &&
           (freq / prescaler) > ADF4360Variants[variant].countersMaxFreq) {
      prescaler *= 2;
    }
    LOGD("setFreq", "prescaler: %u", prescaler);

    uint32_t frequencyRatio;

    // Adjust counters A and B until B >= A and B >= 3
    do {
      tuneRCounter(counterR);
      frequencyPFD   = ref / counterR;
      frequencyRatio = (uint32_t)((float)freq / frequencyPFD + 0.5);
      counterB       = frequencyRatio / prescaler;
      counterA       = frequencyRatio % prescaler;
      // freq = ((prescaler * B) + A) * frequencyPFD;
    } while ((counterA > counterB) && counterB < 3);

    LOGD("setFreq", "A %u, b%u r%u", counterA, counterB, counterR);
  }
  frequency = ((prescaler * counterB) + counterA) * frequencyPFD;
  LOGD("setFreq", "freq: %lu", frequency);

  // Adjust the band select until divided counterR is less than 1MHz
  uint8_t  bandSelectRaw   = 1;
  uint32_t dividedCounterR = frequencyPFD;
  while (dividedCounterR > 1000000 && bandSelectRaw < 8) {
    bandSelectRaw *= 2;
    dividedCounterR = frequencyPFD / bandSelectRaw;
  }
  LOGD("setFreq", "band %u", bandSelectRaw);
  switch (bandSelectRaw) {
    case 1:
      bandSelect = BandSelect_t::ONE;
      break;
    case 2:
      bandSelect = BandSelect_t::TWO;
      break;
    case 4:
      bandSelect = BandSelect_t::FOUR;
      break;
    case 8:
      bandSelect = BandSelect_t::EIGHT;
      break;
    default:
      return MBED_ERROR_INVALID_DATA_DETECTED;
  }
  mbed_error_status_t error = MBED_SUCCESS;

  error = write(Register_t::COUNTER_R);
  if (error)
    return error;
  error = write(Register_t::CONTROL);
  if (error)
    return error;
  wait_us(10e3);
  error = write(Register_t::COUNTER_N);
  if (error)
    return error;

  return MBED_SUCCESS;
}

/**
 * @brief Enable or disable the synth
 *
 * @param enable
 * @return mbed_error_status_t
 */
  mbed_error_status_t ADF4360::setEnable(bool enable) {
  powerDown = enable ? PowerDown_t::ON : PowerDown_t::ASYNC_OFF;
  return write(Register_t::CONTROL);
}

/**
 * @brief Increases the R counter until the PFD frequency is less than the
 * maximum
 *
 * @param counter
 */
void ADF4360::tuneRCounter(uint16_t & counter) {
  uint32_t freq = 0;
  do {
    ++counter;
    freq = ref / counter;
  } while (freq > MAX_FREQ_PFD);
}

/**
 * @brief Write a register to the synth
 *
 * @param reg to write
 * @return mbed_error_status_t
 */
mbed_error_status_t ADF4360::write(Register_t reg) {
  uint32_t buf = 0;
  switch (reg) {
    case Register_t::CONTROL:
      buf = 0;
      buf |= static_cast<uint32_t>(prescaler) << 22;
      buf |= static_cast<uint32_t>(powerDown) << 20;
      buf |= static_cast<uint32_t>(currentSetting2) << 17;
      buf |= static_cast<uint32_t>(currentSetting1) << 14;
      buf |= static_cast<uint32_t>(outputPower) << 12;
      buf |= static_cast<uint32_t>(muteTillLock) << 11;
      buf |= static_cast<uint32_t>(cpGainUse2) << 10;
      buf |= static_cast<uint32_t>(cpOutputTriState) << 9;
      buf |= static_cast<uint32_t>(phaseDetectPositive) << 8;
      buf |= static_cast<uint32_t>(muxOutput) << 5;
      buf |= static_cast<uint32_t>(countersReset) << 4;
      buf |= static_cast<uint32_t>(corePower) << 2;
      break;
    case Register_t::COUNTER_N:
      buf = 2;
      buf |= static_cast<uint32_t>(divide2Select) << 23;
      buf |= static_cast<uint32_t>(divide2) << 22;
      buf |= static_cast<uint32_t>(cpGainUse2) << 21;
      buf |= (static_cast<uint32_t>(counterB) & 0x1FFF) << 8;
      buf |= (static_cast<uint32_t>(counterA) & 0x1F) << 2;
      break;
    case Register_t::COUNTER_R:
      buf = 1;
      buf |= static_cast<uint32_t>(bandSelect) << 20;
      buf |= static_cast<uint32_t>(lockDetectHighPrecision) << 18;
      buf |= static_cast<uint32_t>(antiBacklash) << 16;
      buf |= (static_cast<uint32_t>(counterR) & 0x3FFF) << 2;
      break;
    default:
      return MBED_ERROR_INVALID_DATA_DETECTED;
  }

  // Ignore response, no data can be returned
  cs = 0;
  spi.write((buf >> 16) & 0xFF);
  spi.write((buf >> 8) & 0xFF);
  spi.write((buf >> 0) & 0xFF);
  cs = 1;
  return MBED_SUCCESS;
}