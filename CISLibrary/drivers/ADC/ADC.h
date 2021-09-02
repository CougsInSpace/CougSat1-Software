#ifndef _LIBRARY_DRIVER_ADC_ADC_H_
#define _LIBRARY_DRIVER_ADC_ADC_H_

#include <mbed.h>

enum class ADCChannel_t : uint8_t {
  CM_00 = 0x00,
  CM_01 = 0x01,
  CM_02 = 0x02,
  CM_03 = 0x03,
  CM_04 = 0x04,
  CM_05 = 0x05,
  CM_06 = 0x06,
  CM_07 = 0x07,
  CM_08 = 0x08,
  CM_09 = 0x09,
  CM_10 = 0x0A,
  CM_11 = 0x0B,
  CM_12 = 0x0C,
  CM_13 = 0x0D,
  CM_14 = 0x0E,
  CM_15 = 0x0F,
  DM_00 = 0x80,
  DM_01 = 0x81,
  DM_02 = 0x82,
  DM_03 = 0x83,
  DM_04 = 0x84,
  DM_05 = 0x85,
  DM_06 = 0x86,
  DM_07 = 0x87,
  DM_08 = 0x88,
  DM_09 = 0x89,
  DM_10 = 0x8A,
  DM_11 = 0x8B,
  DM_12 = 0x8C,
  DM_13 = 0x8D,
  DM_14 = 0x8E,
  DM_15 = 0x8F,
  TEMP  = 0xFF
};

class ADC {
public:
  ADC(const ADC &) = delete;
  ADC & operator=(const ADC &) = delete;

  /**
   * @brief Construct a new ADC object
   *
   * @param refVoltage at full counts
   * @param bitDepth of the conversion
   */
  ADC(double refVoltage, uint8_t bitDepth) :
    MAX_RAW((1 << bitDepth) - 1), CONVERSION_FLOAT(1.0 / (1 << bitDepth)) {
    setReferenceVoltage(refVoltage, bitDepth);
  }

  /**
   * @brief Destroy the ADC object
   *
   */
  virtual ~ADC() {};

  /**
   * @brief Read the raw conversion result of a channel
   *
   * @param channel to read
   * @param value to return in counts
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t readRaw(
      ADCChannel_t channel, int32_t & value) = 0;

  /**
   * @brief Tests the ADC is connected and operating properly
   *
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t selfTest() = 0;

  /**
   * @brief Set the reference voltage given the number of bits and vRef
   *
   * @param refVoltage at full counts
   * @param bitDepth of the conversion
   */
  void setReferenceVoltage(double refVoltage, uint8_t bitDepth) {
    this->refVoltage = refVoltage;
    conversionFactor = refVoltage * CONVERSION_FLOAT;
  }

  /**
   * @brief Set the Temperature Conversion Factor
   * temp = (tempSensorVolts - tempOffset) * tempSlope;
   *
   * @param slope Celsius per unit
   * @param offset units at 0 Celsius
   * @param unitVolts will read the voltage of the temperature sensor when true,
   * counts if false
   */
  void setTemperatureFunction(double slope, double offset, bool unitsVolts) {
    tempSlope      = slope;
    tempOffset     = offset;
    tempUnitsVolts = unitsVolts;
  }

  /**
   * @brief Get the reference voltage object
   *
   * @return double vRef in volts
   */
  double getReferenceVoltage() {
    return refVoltage;
  }

  /**
   * @brief Read the voltage of a channel
   * Reads the raw value and multiplies by the conversionFactor
   *
   * @param channel to read
   * @param value to return in volts
   * @return mbed_error_status_t
   */
  mbed_error_status_t readVoltage(ADCChannel_t channel, double & value) {
    int32_t             buf    = 0;
    mbed_error_status_t result = readRaw(channel, buf);
    value                      = (double)buf * conversionFactor;
    return result;
  }

  /**
   * @brief Read the temperature of the ADC
   *
   * @param value to return in Celsius
   * @return mbed_error_status_t
   */
  mbed_error_status_t readTemp(double & value) {
    mbed_error_status_t result;
    if (tempUnitsVolts)
      result = readVoltage(ADCChannel_t::TEMP, value);
    else {
      int32_t buf = 0;
      result      = readRaw(ADCChannel_t::TEMP, buf);
      value       = (double)buf;
    }

    value = (value - tempOffset) * tempSlope;
    return result;
  }

public:
  // Aka bit depth related info
  const int32_t MAX_RAW;
  const double  CONVERSION_FLOAT;

protected:
  double refVoltage       = 0.0; // Volts
  double conversionFactor = 0.0; // Volts per count

  // temp = (tempSensorVolts - tempOffset) * tempSlope;
  double tempSlope      = 0;
  double tempOffset     = 0;
  bool   tempUnitsVolts = true;
};

#endif /* _LIBRARY_DRIVER_ADC_ADC_H_ */