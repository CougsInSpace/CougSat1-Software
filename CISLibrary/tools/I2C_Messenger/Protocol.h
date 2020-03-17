/**
 * @file Protcol.h
 * @author Darren Cheatham
 * @date 16 March 2020
 * @brief Protocol for I2C messages
 */

#include <mbed.h>

/**
 * @brief The max size of a packet
 */
const uint16_t PACKET_MAX_SIZE = 1024;

/**
 * @brief Addresses of subsystems on I2C network
 */
enum Subsystem {
  ADCS = 0x0,
  IFJR = 0x1E,
  IHU,
  PMIC,
  Comms,
  Payload1,
  Payload2,
  Payload3
};

/**
 * @brief How much data and the CRC of the data.
 * When doing a binary transfer, this struct must be the first 8 bytes at the
 * end of the data ptr for ConfigSend. Additional data may be passed, but this
 * *must* be the first 8 bytes.
 */
struct TransferProtocol {
  uint32_t bytes_total;
  int32_t  crc32;
};

/**
 * @brief Used to keep track of an ongoing binary transfer
 */
struct TransferItem {
  TransferProtocol protocol;
  Subsystem        subsystem;
  int8_t           command;
  uint32_t         bytes_sent;
};

/**
 * @brief Represents where, what for, and how much data to send.
 * Primarly used to keep code clean.
 */
struct ConfigSend {
  Subsystem subsystem;
  int8_t    command;
  uint32_t  data_size = 0;
  void *    data      = nullptr;
};

/**
 * @brief Used to receive data
 * Primarly used to keep code clean.
 */
struct ConfigRecv {
  uint32_t data_size = 0;
  void *   data      = nullptr;
};