#include "DRV8837.h"

/**
 * @brief Construct a new DRV8837::DRV8837 object
 *
 * @param fwd pin
 * @param rev pin
 * @param sleep pin
 */
DRV8837::DRV8837(PinName fwd, PinName rev, PinName nSleep) :
  fwd(fwd), rev(rev), nSleep(nSleep, 0) {
  // set the pwm periods
  this->fwd.period_us(DRV8837_PWM_PERIOD_US);
  this->rev.period_us(DRV8837_PWM_PERIOD_US);
}

/**
 * @brief Destroy the DRV8837::DRV8837 object
 *
 */
DRV8837::~DRV8837() {}

/**
 * @brief Set the output of the HBridge
 *
 * @param value [-1.0, 1.0]
 * @param blocking parameter ignored on the DRV8837 implementation
 * @return mbed_error_status_t
 */
mbed_error_status_t DRV8837::set(double value, bool blocking) {
  this->setSleep(false);

  if (value < 0) {
    this->rev = -1.0f * value;
    this->fwd = 0.0f;
  } else {
    this->fwd = (float)value;
    this->rev = 0.0f;
  }

  this->setSleep(true);

  return MBED_SUCCESS;
}

/**
 * @brief Stop the output, brake or coast
 *
 * @param brake will short output if true, high impedance if false
 * @param blocking parameter ignored on the DRV8837 implementation
 * @return mbed_error_status_t
 */
mbed_error_status_t DRV8837::stop(bool brake, bool blocking) {
  this->setSleep(false);
  this->rev = this->fwd = (brake ? 1.0f : 0.0f);
  this->setSleep(true);

  return MBED_SUCCESS;
}

/**
 * @brief Sets the HBridge operating mode
 * @param sleep sets the operating mode to sleep if true, and awakens the
 * HBridge is false.
 */
void DRV8837::setSleep(bool sleep) {
  this->nSleep = !sleep;
}