#ifndef SRC_SUBSYSTEMS_SUBSYSTEM_H_
#define SRC_SUBSYSTEMS_SUBSYSTEM_H_

#include <mbed.h>

class Subsystem {
public:
  Subsystem(const Subsystem &) = delete;
  Subsystem & operator=(const Subsystem &) = delete;

  /**
   * @brief Construct a new Subsystem object
   *
   */
  Subsystem() {}

  /**
   * @brief Destroy the Subsystem object
   *
   */
  ~Subsystem() {}

  /**
   * Intializes subsystem
   * @return error code
   */
  virtual mbed_error_status_t initialize() = 0;

  /**
   * @brief Get an signed int32 from a buffer
   * 
   * @param buffer Buffer
   * @param offset Offset byte
   * @return int32_t 
   */
  static int32_t Int32FromBuffer (char* buffer, int offset=0) {
    return buffer[offset + 0] << 24 | buffer[offset + 1] << 16 | buffer[offset + 2] << 8 | buffer[offset + 3];
  }
};

#endif /* SRC_SUBSYSTEMS_SUBSYSTEM_H_ */