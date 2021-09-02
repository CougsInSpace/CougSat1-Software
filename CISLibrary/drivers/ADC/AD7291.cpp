#include "AD7291.h"

#include "CISConsole.h"

/**
 * @brief Construct a new AD7291::AD7291 object
 *
 * @param i2c connected to the ADC
 * @param addr address of the ADC
 * @param refVoltage reference voltage in volts
 * @param tempSlope Celsius per count
 * @param tempOffset counts at 0 Celsius
 */
AD7291::AD7291(I2C & i2c, AD7291Addr_t addr, double refVoltage,
    double tempSlope, double tempOffset) :
  ADC(refVoltage, BIT_DEPTH),
  i2c(i2c), addr(addr) {
  setTemperatureFunction(tempSlope, tempOffset, false);
}

/**
 * @brief Destroy the AD7291::AD7291 object
 *
 */
AD7291::~AD7291() {}

/**
 * @brief Read the raw conversion result of a channel
 *
 * @param channel to read
 * @param value to return in counts
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7291::readRaw(ADCChannel_t channel, int32_t & value) {
  uint16_t            raw   = 0;
  mbed_error_status_t error = MBED_SUCCESS;
  if (channel == ADCChannel_t::TEMP) {
    error = read(Register_t::RESULT_TEMP, raw);
    if (error) {
      LOGE("AD7291", "Failed to read RESULT_TEMP: 0x%08X", error);
      return error;
    }

    if ((raw >> 12) != 0x8) {
      LOGE("AD7291", "Read result is not temp channel");
      return MBED_ERROR_INVALID_DATA_DETECTED;
    }

    // Remove channel ID bits
    raw = raw & 0x00000FFF;

    // Set the MSB bits to the same as the 11th
    // Handles negative temperatures
    value = raw | (0xFFFFF000 * ((raw >> 11) & 0x1));
  } else if (channel < ADCChannel_t::CM_08) {
    // Set the control's channel to the selected channel
    channels = ((uint8_t)0x80) >> static_cast<uint8_t>(channel);
    error    = writeControlRegister();
    if (error) {
      LOGE("AD7291", "Failed to write control register: 0x%08X", error);
      return error;
    }

    error = read(Register_t::RESULT_VOLTAGE, raw);
    if (error) {
      LOGE("AD7291", "Failed to read RESULT_VOLTAGE: 0x%08X", error);
      return error;
    }

    if ((raw >> 12) != static_cast<uint8_t>(channel)) {
      LOGE("AD7291", "Read result is not selected channel");
      return MBED_ERROR_INVALID_DATA_DETECTED;
    }

    value = raw & 0x00000FFF;
  } else {
    LOGE("AD7291", "Selected channel is not available to read");
    return MBED_ERROR_INVALID_ARGUMENT;
  }
  return MBED_SUCCESS;
}

/**
 * @brief Tests the I2C interface to the IC by writing the config register
 * (checks for ACK)
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7291::selfTest() {
  return writeControlRegister();
}

/**
 * @brief Reset all registers to their default value
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7291::reset() {
  mbed_error_status_t error = write(Register_t::CONTROL, 2);
  if (error) {
    LOGE("AD7291", "Failed to write CONTROL: 0x%08X", error);
    return error;
  }
  channels          = 0;
  tempSense         = true;
  noiseDelayed      = true;
  externalReference = false;
  alertActiveLow    = false;
  clearAlert        = false;
  autocycle         = false;

  error = writeControlRegister();
  if (error) {
    LOGE("AD7291", "Failed to write CONTROL register: 0x%08X", error);
    return error;
  }
  return MBED_SUCCESS;
}

/**
 * @brief Read the value of a register
 *
 * @param register to read
 * @param value to return
 * @param registerOffset (used when reading DATA_*_CH0-7)
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7291::read(
    Register_t reg, uint16_t & value, uint8_t registerOffset) {
  char buf[2];
  buf[0] = static_cast<char>(reg) + registerOffset;
  if (i2c.write(static_cast<int>(addr), buf, 1))
    return MBED_ERROR_WRITE_FAILED;

  if (i2c.read(static_cast<int>(addr), buf, 2))
    return MBED_ERROR_READ_FAILED;

  value = buf[0] << 8 | buf[1];
  return MBED_SUCCESS;
}

/**
 * @brief Write the value of a register
 *
 * @param register to write
 * @param value to write
 * @param registerOffset (used when writing DATA_*_CH0-7)
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7291::write(
    Register_t reg, uint16_t value, uint8_t registerOffset) {
  char buf[3];
  buf[0] = static_cast<char>(reg) + registerOffset;
  buf[1] = (value >> 8) & 0xFF;
  buf[2] = (value >> 0) & 0xFF;

  if (i2c.write(static_cast<int>(addr), buf, 3))
    return MBED_ERROR_WRITE_FAILED;
  return MBED_SUCCESS;
}

/**
 * @brief Write the control register
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7291::writeControlRegister() {
  uint16_t buf = 0;

  buf = buf | (channels << 8);
  buf = buf | (static_cast<uint16_t>(tempSense) << 7);
  buf = buf | (static_cast<uint16_t>(noiseDelayed) << 5);
  buf = buf | (static_cast<uint16_t>(externalReference) << 4);
  buf = buf | (static_cast<uint16_t>(alertActiveLow) << 3);
  buf = buf | (static_cast<uint16_t>(clearAlert) << 2);
  buf = buf | (static_cast<uint16_t>(autocycle) << 0);

  return write(Register_t::CONTROL, buf);
}