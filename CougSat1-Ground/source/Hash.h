#ifndef _HASH_H_
#define _HASH_H_

#include <stdint.h>
#include <string>

typedef uint32_t HashValue_t;

class Hash {
public:
  Hash(HashValue_t value = 0xFFFFFFFF);
  virtual ~Hash();

  /**
   * @brief Add a character to the hash
   *
   * @param c to add
   */
  virtual void add(const char c) {
    value = calculateHash(value, c);
  }

  void   add(const char * c, size_t length);
  size_t add(const char * c, size_t length, const char end);
  size_t add(const unsigned char * c, size_t length, const char end);

  /**
   * @brief Add a string to the hash
   *
   * @param str to hash
   */
  inline void add(const std::string & str) {
    add(str.c_str(), str.length());
  }

  const HashValue_t get() const;

  /**
   * @brief Calculate the hash from a string
   *
   * @param str to hash
   * @return HashValue_t hash
   */
  static HashValue_t calculateHash(const std::string & str) {
    HashValue_t hash = 0xFFFFFFFF;
    for (char c : str) {
      hash = calculateHash(hash, c);
    }
    return finishHash(hash);
  }

  /**
   * @brief Calculate the hash from a string
   *
   * @param string to hash
   * @return constexpr HashValue_t hash
   */
  static constexpr HashValue_t calculateHash(char * string) {
    HashValue_t hash = 0xFFFFFFFF;
    while (*string != '\0') {
      hash = calculateHash(hash, *string);
      ++string;
    }
    return finishHash(hash);
  }

private:
  /**
   * @brief Calculate the hash through its algorithm on the seed hash and char
   * Jenkin's hash function
   *
   * @param hash to seed
   * @param c char to append
   * @return constexpr HashValue_t hash
   */
  static constexpr HashValue_t calculateHash(HashValue_t hash, const char c) {
    hash = static_cast<HashValue_t>(hash + static_cast<uint64_t>(c));
    hash = static_cast<HashValue_t>(hash + (static_cast<uint64_t>(hash) << 10));
    hash = static_cast<HashValue_t>(hash ^ (static_cast<uint64_t>(hash) >> 6));
    return hash;
  }

  /**
   * @brief Applies final operations to finish creating a hash
   *
   * @param hash to process
   * @return constexpr HashValue_t final hash
   */
  static constexpr HashValue_t finishHash(HashValue_t hash) {
    hash = static_cast<HashValue_t>(hash + (static_cast<uint64_t>(hash) << 3));
    hash = static_cast<HashValue_t>(hash ^ (static_cast<uint64_t>(hash) >> 11));
    hash = static_cast<HashValue_t>(hash + (static_cast<uint64_t>(hash) << 15));
    return hash;
  }

  HashValue_t value;
};

#endif /* _HASH_H_ */