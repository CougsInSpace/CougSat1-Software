/**
 * @file MessageMaster.h
 * @author Darren Cheatham
 * @date 16 March 2020
 * @brief Wrapper for I2C Master Message Protocol
 */

#include "Protocol.h"

#include <I2C.h>
#include <mbed.h>
#include <vector>

class MessageMaster {
protected:
  /**
   * @brief I2C bus ptr
   */
  I2C * bus_i2c = nullptr;

  /**
   * @brief Keeps track of a data transfer
   */
  TransferItem transfer_item;

public:
  /**
   * @brief Construct a new Message Master object
   *
   * @param sda SDA Pin
   * @param scl SCL Pin
   */
  MessageMaster(PinName sda, PinName scl);

  /**
   * @brief Destroy the Message Master object
   */
  ~MessageMaster();

  /**
   * @brief Send a command to a subsystem
   *
   * @param config_send
   * @return mbed_error_status_t
   */
  mbed_error_status_t Send(ConfigSend & config_send);

  /**
   * @brief Send a command to a subsystem and write the response at the ptr in
   * the ConfigRecv struct
   *
   * @param subsystem
   * @param command
   * @return mbed_error_status_t
   */
  mbed_error_status_t Send(ConfigSend & config_send, ConfigRecv & config_recv);

  /**
   * @brief Starts a binary transfer
   * The first sizeof(TransferProtocl) bytes *must* be the transfer protocol
   * struct. Any additional data may be added after.
   *
   * @param config_send Transfer inital message
   * @return mbed_error_status_t
   */
  mbed_error_status_t TransferStart(ConfigSend & config_send);

  /**
   * @brief How many bytes the slave will expect on the next write
   *
   * @return uint32_t
   */
  inline uint32_t TransferWriteSize();

  /**
   * @brief Emit a block of data to the slave
   *
   * @param config_send Transfer inital message
   * @return mbed_error_status_t
   */
  mbed_error_status_t TransferEmit(ConfigSend & config_send);

  /**
   * @brief Ends a binary transfer
   *
   * @return mbed_error_status_t
   */
  mbed_error_status_t TransferEnd();
};