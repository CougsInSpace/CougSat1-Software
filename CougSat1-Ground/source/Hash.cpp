#include "Hash.h"

/**
 * @brief Construct a new Hash:: Hash object
 * Create a new referenceCount equal to 1
 *
 * @param value to initialize the hash to
 */
Hash::Hash(HashValue_t value) : value(value) {}

/**
 * @brief Destroy the Result object
 */
Hash::~Hash() {}

/**
 * @brief Add a character array until the termination character or length number
 * of characters
 *
 * @param c array to add
 * @param end character will stop and not hash this character
 * @return size_t number of characters read
 */
size_t Hash::add(const char * c, size_t length, const char end) {
  size_t count = 0;
  while (*c != end && length > 0) {
    add(*c);
    --length;
    ++c;
    ++count;
  }
  return count;
}

/**
 * @brief Add a character array until the termination character or length number
 * of characters
 *
 * @param c array to add
 * @param end character will stop and not hash this character
 * @return size_t number of characters read
 */
size_t Hash::add(const unsigned char * c, size_t length, const char end) {
  size_t count = 0;
  while (*c != end && length > 0) {
    add(*c);
    --length;
    ++c;
    ++count;
  }
  return count;
}

/**
 * @brief Add a character array, length characters long
 *
 * @param c array to add
 * @param length number of characters
 */
void Hash::add(const char * c, size_t length) {
  while (length > 0) {
    add(*c);
    --length;
    ++c;
  }
}

/**
 * @brief Get the current hash value
 *
 * @return const HashValue_t hash
 */
const HashValue_t Hash::get() const {
  return finishHash(value);
}