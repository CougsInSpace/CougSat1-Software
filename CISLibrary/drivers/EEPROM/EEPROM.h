#ifndef _LIBRARY_DRIVER_EEPROM_EEPROM_H_
#define _LIBRARY_DRIVER_EEPROM_EEPROM_H_

#include <mbed.h>

class EEPROM {
public:
  EEPROM(const EEPROM &) = delete;
  EEPROM & operator=(const EEPROM &) = delete;

  /**
   * @brief Construct a new EEPROM object
   *
   */
  EEPROM() {}

  /**
   * @brief Destroy the EEPROM object
   *
   */
  virtual ~EEPROM() {};

  /**
   * @brief Read a byte
   *
   * @param addr of byte
   * @param data to return result
   * @param blocking will wait until operation is complete if true
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t read(
      uint32_t addr, uint8_t & data, bool blocking = true) = 0;

  /**
   * @brief Write a byte
   *
   * @param addr of byte
   * @param data to write
   * @param blocking will wait until operation is complete if true
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t write(
      uint32_t addr, uint8_t data, bool blocking = true) = 0;
};

#endif /* _LIBRARY_DRIVER_EEPROM_EEPROM_H_ */