#ifndef _LIBRARY_DRIVER_HBRIDGE_HBRIDGE_H_
#define _LIBRARY_DRIVER_HBRIDGE_HBRIDGE_H_

#include <mbed.h>

class HBridge {
public:
  HBridge(const HBridge &) = delete;
  HBridge & operator=(const HBridge &) = delete;

  /**
   * @brief Construct a new HBridge object
   *
   */
  HBridge() {}

  /**
   * @brief Destroy the HBridge object
   *
   */
  virtual ~HBridge() {};

  /**
   * @brief Set the output of the HBridge
   *
   * @param value [-1.0, 1.0]
   * @param blocking will wait until operation is complete if true
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t set(double value, bool blocking = true) = 0;

  /**
   * @brief Stop the output, brake or coast
   *
   * @param brake will short output if true, high impedance if true
   * @param blocking will wait until operation is complete if true
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t stop(bool brake = true, bool blocking = true) = 0;
};

#endif /* _LIBRARY_DRIVER_HBRIDGE_HBRIDGE_H_ */