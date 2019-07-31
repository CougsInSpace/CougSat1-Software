#include "DRV8837.h"

/**
 * @brief Construct a new DRV8837::DRV8837 object
 *
 * @param fwd pin
 * @param rev pin
 * @param sleep pin
 */
DRV8837::DRV8837(PinName fwd, PinName rev, PinName sleep) :
  fwd(fwd), rev(rev), sleep(sleep) {}

/**
 * @brief Destroy the DRV8837::DRV8837 object
 *
 */
DRV8837::~DRV8837() {}

/**
 * @brief Set the output of the HBridge
 *
 * @param value [-1.0, 1.0]
 * @param blocking will wait until operation is complete if true
 * @return CISResult_t error code
 */
CISResult_t DRV8837::set(double value, bool blocking) {
  return {ERROR_NOT_SUPPORTED, ""};
}

/**
 * @brief Stop the output, brake or coast
 *
 * @param brake will short output if true, high impedance if true
 * @param blocking will wait until operation is complete if true
 * @return CISResult_t
 */
CISResult_t DRV8837::stop(bool brake, bool blocking) {
  return {ERROR_NOT_SUPPORTED, ""};
}