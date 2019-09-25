#ifndef _LIBRARY_DRIVER_FLASH_FLASH_H_
#define _LIBRARY_DRIVER_FLASH_FLASH_H_

#include <mbed.h>

class Flash {
public:
  Flash(const Flash &) = delete;
  Flash & operator=(const Flash &) = delete;

  /**
   * @brief Construct a new Flash object
   *
   * @param blockSize in bytes
   */
  Flash(uint16_t blockSize) : blockSize(blockSize) {}

  /**
   * @brief Destroy the Flash object
   *
   */
  virtual ~Flash() {};

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
   * @brief Write a block of data
   *
   * @param address of first byte
   * @param data to write (length is blockSize)
   * @param blocking will wait until operation is complete if true
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t writeBlock(
      uint32_t addr, uint8_t * data, bool blocking = true) = 0;

protected:
  uint16_t blockSize = 0;
};

#endif /* _LIBRARY_DRIVER_FLASH_FLASH_H_ */