#include "AD7689.h"

#include "CISConsole.h"

/**
 * @brief Construct a new AD7689::AD7689 object
 *
 * @param mosi master out, slave in pin
 * @param miso master in, slave out pin
 * @param sclk serial clock pin
 * @param cnv pin (chip select)
 * @param refVoltage reference voltage in volts
 */
AD7689::AD7689(
    PinName mosi, PinName miso, PinName sclk, PinName cnv, double refVoltage) :
  ADC(refVoltage, BIT_DEPTH),
  spi(mosi, miso, sclk, cnv, use_gpio_ssel) {}

/**
 * @brief Destroy the AD7689::AD7689 object
 *
 */
AD7689::~AD7689() {}

/**
 * @brief Read the raw conversion result of a channel
 *
 * @param channel to read
 * @param value to return in counts
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7689::readRaw(ADCChannel_t channel, int32_t & value) {
  uint16_t            raw   = 0;
  mbed_error_status_t error = MBED_SUCCESS;
  if (channel == ADCChannel_t::TEMP) {
    inputConfig = InputConfig_t::TEMP;
  } else if ((static_cast<uint8_t>(channel) & 0x7F) <
             static_cast<uint8_t>(ADCChannel_t::CM_08)) {
    // Set the control's channel to the selected channel
    inputChannel = static_cast<uint8_t>(channel);
    if (inputChannel < 0x80)
      inputConfig = InputConfig_t::UNIPOLAR_COM;
    else
      inputConfig = InputConfig_t::BIPOLAR_DIFF;
  } else {
    ERROR("AD7291", "Selected channel is not available to read");
    return MBED_ERROR_INVALID_ARGUMENT;
  }
  uint16_t config = getConfigRegister();

  error = transfer(config, (uint16_t *)nullptr);
  if (error) {
    ERROR("AD7689", "Failed to transfer config");
    return error;
  }

  // Discard second byte
  error = transfer(config, (uint16_t *)nullptr);
  if (error) {
    ERROR("AD7689", "Failed to skip second byte");
    return error;
  }

  uint16_t raw = 0;
  error        = transfer(config, &raw);
  if (error) {
    ERROR("AD7689", "Failed to read data");
    return error;
  }

  value = static_cast<int32_t>(raw);
  if (inputConfig == InputConfig_t::BIPOLAR_DIFF)
    // Extend the sign for two's complement values
    value = value | (0xFFFF0000 * ((value >> 15) & 0x1));

  return MBED_SUCCESS;
}

/**
 * @brief Read the temperature of the ADC
 *
 * @param value to return in Celsius
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7689::readTemp(double & value) {
  mbed_error_status_t result = readVoltage(ADCChannel_t::TEMP, value);

  value = (value - TEMP_OFFSET) * TEMP_SLOPE;
  return result;
}

/**
 * @brief Tests the SPI interface to the IC by writing the register then reading
 * it back
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7689::selfTest() {
  uint16_t            config = getConfigRegister(true);
  uint32_t            bufRX  = 0;
  mbed_error_status_t error  = transfer(config, (uint16_t *)nullptr);
  if (error) {
    ERROR("AD7689", "Failed to transfer config");
    return error;
  }

  // Discard second byte
  error = transfer(config, (uint16_t *)nullptr);
  if (error) {
    ERROR("AD7689", "Failed to skip second byte");
    return error;
  }

  error = transfer(config, &bufRX);
  if (error) {
    ERROR("AD7689", "Failed to read data");
    return error;
  }

  if ((bufRX & 0xFFFF) == config)
    return MBED_SUCCESS;

  ERROR("AD7689", "Readback did not match");
  return MBED_ERROR_INVALID_DATA_DETECTED;
}

/**
 * @brief Simultaneously write and read data to/from the ADC
 *
 * @param write register, usually from getControlRegister
 * @param read result, write only if nullptr
 * @param conversionDelay will wait for a conversion to take place before
 * returning
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7689::transfer(
    const uint16_t write, uint16_t * read, bool conversionDelay) {
  spi.select();
  char bufTX[2] = {(write >> 8), (write & 0xFF)};
  char bufRX[2] = {0};
  int  result   = spi.write(bufTX, 2, bufRX, 2);
  // Should return that it wrote and read 2 bytes
  if (result != 2) {
    spi.deselect();
    return MBED_ERROR_WRITE_FAILED;
  }
  if (read != nullptr)
    *read = (bufRX[1] << 8) | bufRX[0];
  spi.deselect();

  if (conversionDelay)
    wait_us(DELAY_CNV_US);

  return MBED_SUCCESS;
}

/**
 * @brief Simultaneously write and read data to/from the ADC
 *
 * @param write register, usually from getControlRegister
 * @param read result, write only if nullptr
 * @param conversionDelay will wait for a conversion to take place before
 * returning
 * @return mbed_error_status_t
 */
mbed_error_status_t AD7689::transfer(
    const uint16_t write, uint32_t * read, bool conversionDelay) {
  spi.select();
  char bufTX[4] = {(write >> 8), (write & 0xFF), 0, 0};
  char bufRX[4] = {0};
  int  result   = spi.write(bufTX, 4, bufRX, 4);
  // Should return that it wrote and read 4 bytes
  if (result != 4) {
    spi.deselect();
    return MBED_ERROR_WRITE_FAILED;
  }
  if (read != nullptr)
    *read = (bufRX[3] << 24) | (bufRX[2] << 16) | (bufRX[1] << 8) | bufRX[0];
  spi.deselect();

  if (conversionDelay)
    wait_us(DELAY_CNV_US);

  return MBED_SUCCESS;
}

/**
 * @brief Get the config register from the settings
 * Config register is 14b and is left aligned in the 16b return (bits[1:0] are
 * not useful)
 *
 * @param readback will shift the config register out after the data
 * @return uint16_t register[15:2] as defined in table 11 of datasheet
 */
uint16_t AD7689::getConfigRegister(bool readback) {
  uint16_t buf = 0x20; // Overwrite config register
  buf          = buf | (static_cast<uint16_t>(inputConfig) << 10);
  buf          = buf | (static_cast<uint16_t>(inputChannel & 0x7) << 7);
  buf          = buf | (static_cast<uint16_t>(lowPassFilter) << 6);
  buf          = buf | (static_cast<uint16_t>(refConfig) << 3);
  buf          = buf | (static_cast<uint16_t>(channelSeq) << 1);
  buf          = buf | (static_cast<uint16_t>(readback) << 0);
  return buf;
}