#include "AD7689.h"

/**
 * @brief Construct a new AD7689::AD7689 object
 *
 * @param spi connected to the ADC
 * @param cnv pin (chip select)
 * @param refVoltage reference voltage in volts
 */
AD7689::AD7689(SPI & spi, PinName cnv, double refVoltage) :
  ADC(refVoltage, BIT_DEPTH), spi(spi), cnv(cnv) {}

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
 * @param blocking will wait until data is present if true
 * @return CISResult_t error code
 */
CISResult_t AD7689::readRaw(
    ADCChannel_t channel, int32_t & value, bool blocking) {
  return {ERROR_NOT_SUPPORTED, ""};
}