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
};

#endif /* SRC_SUBSYSTEMS_SUBSYSTEM_H_ */