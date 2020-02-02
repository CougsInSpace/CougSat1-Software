#ifndef _LIBRARY_DRIVER_SYNTH_SYNTH_H_
#define _LIBRARY_DRIVER_SYNTH_SYNTH_H_

#include <mbed.h>

class Synth {
public:
  Synth(const Synth &) = delete;
  Synth & operator=(const Synth &) = delete;

  /**
   * @brief Construct a new Synth object
   *
   * uin32_t ref clock in Hertz
   */
  Synth(const uint32_t ref) : ref(ref) {}

  /**
   * @brief Destroy the Synth object
   *
   */
  virtual ~Synth() {};

  /**
   * @brief Set the frequency outputted from the synth
   *
   * @param freq in Hertz
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t setFrequency(uint32_t freq) = 0;

  /**
   * @brief Enable or disable the synth
   *
   * @param enable
   * @return mbed_error_status_t
   */
  virtual mbed_error_status_t setEnable(bool enable) = 0;

  /**
   * @brief Get the frequency of the synth
   *
   * @return uint32_t in Hertz
   */
  uint32_t getFrequency() const {
    return frequency;
  }

protected:
  const uint32_t ref       = 0;
  uint32_t       frequency = 0;
};

#endif /* _LIBRARY_DRIVER_SYNTH_SYNTH_H_ */