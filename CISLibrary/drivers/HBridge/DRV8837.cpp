#include "DRV8837.h"

/**
 * @brief Construct a new DRV8837::DRV8837 object
 *
 * @param fwd pin
 * @param rev pin
 * @param sleep pin
 */
DRV8837::DRV8837(PinName fwd, PinName rev, PinName nSleep)
  : fwd(fwd), rev(rev), nSleep(nSleep)
{
  // set the pwm frequencies
  this->fwd.period(1.0 / DRV8837_PWM_FREQ_HZ);
  this->rev.period(1.0 / DRV8837_PWM_FREQ_HZ);

  // disable initially
  this->nSleep.write(1);
}

/**
 * @brief Destroy the DRV8837::DRV8837 object
 *
 */
DRV8837::~DRV8837() { }

/**
 * @brief Set the output of the HBridge
 *
 * @param value [-1.0, 1.0]
 * @param blocking will wait until operation is complete if true
 * @return mbed_error_status_t
 */
mbed_error_status_t DRV8837::set(double value) {
  if (value < -1.0 || value > 1.0) {
    return MBED_ERROR_CODE_INVALID_ARGUMENT;
  }

  this->wake();

  if (value == 0) {
    return this->stop(false);
  } else {
    if (value < 0) {
      this->rev = -value;
      this->fwd = 0.0;
    } else {
      this->fwd = value;
      this->rev = 0.0;
    }
  }

  wait_us(DRV8837_SET_WAIT_TIME_US);
  this->sleep_immediate();

  return MBED_SUCCESS;
}

/**
 * @brief Stop the output, brake or coast
 *
 * @param brake will short output if true, high impedance if false
 * @param blocking will wait until operation is complete if true
 * @return mbed_error_status_t
 */
mbed_error_status_t DRV8837::stop(bool brake) {
  this->wake();
  this->rev = this->fwd = (brake ? 1.0 : 0.0);

  wait_us(DRV8837_SET_WAIT_TIME_US);
  this->sleep_immediate();

  return MBED_SUCCESS;
}

/**
 * @brief Wakes the Hbridge.
 * 
 */
void DRV8837::wake() {
  this->nSleep = 0;
}

/**
 * @brief Force the Hbridge to sleep.
 * 
 */
void DRV8837::sleep_immediate() {
  this->nSleep = 1;
}