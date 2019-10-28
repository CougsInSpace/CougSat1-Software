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
   * @brief Converts an array of bytes to an integer type
   * 
   * @tparam T Integer
   * @param buffer Bytes
   * @param buffer_size Amount of Bytes
   * @param offset Offset in Bytess
   * @return T Value
   */
  template<typename T>
  static T BufferToInt (char* buffer, size_t buffer_size, int offset=0) {
      int8_t type_width = sizeof(T) * 8;

      if (buffer_size - offset > type_width) {
          //It won't work. Throw error?
          return (T)-1;
      }

      T value = 0;
      for (int8_t pos = 0; pos < (type_width / 8); pos++) {
          value = value | buffer[offset + pos] << (type_width - ((pos + 1) * 8));
      }

      return value;
  }

  /**
   * @brief Converts an array of bytes to an array of integers
   * 
   * @tparam T type
   * @param array Array to populate
   * @param array_size Size to populate
   * @param buffer Bytes
   * @param buffer_size Amount of Bytes
   * @param offset Offset in Bytes
   */
  template<typename T>
  static void BufferToArray (T* array, size_t array_size, char* buffer, size_t buffer_size, int offset=0) {
      const size_t data_width = sizeof(T);

      if (buffer_size - offset > array_size * data_width) {
          return;
      }

      for (int8_t pos = 0; pos < array_size; pos++) {
          *(array + pos) = BufferToInt<T>(buffer, buffer_size, pos * data_width);
      }
  }
};

#endif /* SRC_SUBSYSTEMS_SUBSYSTEM_H_ */