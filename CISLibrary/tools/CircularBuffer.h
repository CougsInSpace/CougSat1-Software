#ifndef _LIBRARY_TOOLS_CIRCULAR_BUFFER_H_
#define _LIBRARY_TOOLS_CIRCULAR_BUFFER_H_

#include <ResultCode.h>
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
   * @param element to return
   * @return ResultCode_t
   */
  ResultCode_t pop(T & element) {
    if (indexRead == indexWrite)
      return ResultCode_t::BUFFER_OVERFLOW;
    element = data[indexRead];
    ++indexRead;
    return ResultCode_t::SUCCESS;
  }

  /**
   * @brief Push an element into the buffer
   *
   * @param element to push
   * @return ResultCode_t
   */
  ResultCode_t push(const T & element) {
    if (((indexWrite + 1) & 0xFF) == indexRead)
      return ResultCode_t::BUFFER_OVERFLOW;
    data[indexWrite] = element;
    ++indexWrite;
    return ResultCode_t::SUCCESS;
  }

  /**
   * @brief Get the number of elements in the buffer
   *
   * @return uint8_t
   */
  uint8_t size() {
    return indexWrite - indexRead;
  }
};

#endif /* _LIBRARY_TOOLS_CIRCULAR_BUFFER_H_ */