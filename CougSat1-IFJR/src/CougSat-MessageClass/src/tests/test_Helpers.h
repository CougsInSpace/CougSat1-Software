/**
 * @brief Helper function to assemble paackets
 * 
 * @tparam T type
 * @param packet_pos 
 * @param type 
 * @param count Amount of items to copy
 */
template <typename T>
void TEST_MemcpyHelper(uint8_t*& packet_pos, T* type, size_t count) {
  size_t bytes = count * sizeof(T);
  memcpy(packet_pos, type, bytes);
  packet_pos += bytes;
}