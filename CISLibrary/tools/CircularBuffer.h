#ifndef _LIBRARY_TOOLS_CIRCULAR_BUFFER_H_
#define _LIBRARY_TOOLS_CIRCULAR_BUFFER_H_

#include <stdexcept>
#include <cstdint>

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
   * @return true if element was popped, false otherwise
   */
  inline bool pop(T & element) {
    if (indexRead == indexWrite)
      return false;
    element = data[indexRead];
    ++indexRead;
    return true;
  }

  /**
   * @brief Push an element into the buffer
   *
   * @param element to push
   * @return bool true if element was pushed, false otherwise
   */
  inline bool push(const T & element) {
    if (((indexWrite + 1) & 0xFF) == indexRead)
      return false;
    data[indexWrite] = element;
    ++indexWrite;
    return true;
  }

  /**
   * @brief Push an element into the buffer, replace the oldest if full
   *
   * @param element to push
   */
  inline void pushReplace(const T & element) {
    if (((indexWrite + 1) & 0xFF) == indexRead)
      ++indexRead;
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