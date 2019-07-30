#include "AD7291.h"

/**
 * @brief Construct a new AD7291::AD7291 object
 *
 * @param i2c connected to the ADC
 * @param addr address of the ADC
 * @param refVoltage reference voltage in volts
 */
AD7291::AD7291(I2C & i2c, AD7291Addr_t addr, double refVoltage) :
  ADC(refVoltage, BIT_DEPTH), i2c(i2c), addr(addr) {}

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
 * @return CISResult_t error code
 */
CISResult_t AD7291::readRaw(ADCChannel_t channel, int32_t & value) {
  return {ERROR_NOT_SUPPORTED, ""};
}