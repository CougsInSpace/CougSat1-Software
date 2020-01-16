#ifndef _LIBRARY_TOOLS_CIRCULAR_BUFFER_H_
#define _LIBRARY_TOOLS_CIRCULAR_BUFFER_H_

#include <stdexcept>
#include <stdint.h>

struct PairDouble_t {
  double a;
  double b;
};

struct PairInt16_t {
  int16_t a;
  int16_t b;
};

template <typename T> class CircularBuffer {
private:
  T       data[256];
  uint8_t indexRead  = 0;
  uint8_t indexWrite = 0;

public:
  /**
   * @brief Construct a new Circular Buffer object
   *
   */
  CircularBuffer() {}

  /**
   * @brief Pop an element off the buffer
   *
   * @return T element to return
   */
  inline T pop() {
    if (indexRead == indexWrite)
      throw std::underflow_error("Buffer is empty");
    T element = data[indexRead];
    ++indexRead;
    return element;
  }

  /**
   * @brief Push an element into the buffer
   *
   * @param element to push
   */
  inline void push(const T & element) {
    if (((indexWrite + 1) & 0xFF) == indexRead)
      throw std::overflow_error("Buffer is full");
    data[indexWrite] = element;
    ++indexWrite;
  }

  /**
   * @brief Get the number of elements in the buffer
   *
   * @return uint8_t
   */
  inline uint8_t size() {
    return indexWrite - indexRead;
  }
};

#endif /* _LIBRARY_TOOLS_CIRCULAR_BUFFER_H_ */