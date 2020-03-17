/**
 * @file MessageMaster.cpp
 * @author Darren Cheatham
 * @date 16 March 2020
 * @brief Wrapper for I2C Master Message Protocol
 */

#include "MessageMaster.h"

MessageMaster::MessageMaster(PinName sda, PinName scl) {
  bus_i2c = new I2C(sda, scl);
}

MessageMaster::~MessageMaster() {
  delete bus_i2c;
}

/**
 * @brief Sends a command to the subsystem.
 * This first sends the command byte, ack's, then if data_size > 0 sends any
 * data found at config_send.data. Returns the ack.
 *
 * @param config_send
 * @return mbed_error_status_t
 */
mbed_error_status_t MessageMaster::Send(ConfigSend & config_send) {
  // Write the command code
  // TODO: Look into why a char is a different size than a int8_t
  int32_t code = bus_i2c->write(config_send.subsystem,
      (const char *)config_send.command, sizeof(config_send.command));

  if (code != MBED_SUCCESS) {
    return code;
  }

  // If there is data, then write the data:
  if (config_send.data_size != 0 && config_send.data != nullptr) {
    code = bus_i2c->write(config_send.subsystem, (const char *)config_send.data,
        config_send.data_size);
  }

  return code;
}

/**
 * @brief Send a command to a subsystem and write the response at the ptr in the
 * ConfigRecv struct. Calls Send(config_send) to send the command, then
 * ReadAddressed's the slave to read the data. The data is read directly into
 * the buffer in config_recv for the specified data_size.
 *
 * @param config_send
 * @param config_recv
 * @return mbed_error_status_t
 */
mbed_error_status_t MessageMaster::Send(
    ConfigSend & config_send, ConfigRecv & config_recv) {
  int32_t code = Send(config_send);

  if (code != MBED_SUCCESS) {
    return code;
  }

  if (config_recv.data_size == 0 || config_recv.data == nullptr) {
    // TODO: Find the right error code
    return -1;
  }

  code = bus_i2c->read(
      config_send.subsystem, (char *)config_recv.data, config_recv.data_size);

  return code;
}

/**
 * @brief Starts a binary transfer
 * Zeros out transfer_item field, then coppies the TransferProtocol over
 * transfer_item. Configuration from config_send is loaded, then
 * Send(config_send) is called & returned.
 *
 * @param config_send
 * @return mbed_error_status_t
 */
mbed_error_status_t MessageMaster::TransferStart(ConfigSend & config_send) {
  if (config_send.data_size < sizeof(TransferItem)) {
    // TransferProtocol struct *must* be the first 8 bytes.
    return -1;
  }

  bzero((void *)&transfer_item, sizeof(TransferItem));
  memcpy((void *)&transfer_item, (void *)config_send.data,
      sizeof(TransferProtocol));

  transfer_item.subsystem = config_send.subsystem;
  transfer_item.command   = config_send.command;

  return Send(config_send);
}

/**
 * @brief How many bytes the slave will expect on the next write
 *
 * @return uint32_t
 */
inline uint32_t MessageMaster::TransferWriteSize() {
  return transfer_item.protocol.bytes_total >
                 (transfer_item.bytes_sent + PACKET_MAX_SIZE)
             ? PACKET_MAX_SIZE
             : (transfer_item.protocol.bytes_total - transfer_item.bytes_sent);
}

/**
 * @brief Emit a block of data to the slave
 * WriteAddressed's the slave for TransferWriteSize() bytes.
 *
 * @param config_send
 * @return mbed_error_status_t
 */
mbed_error_status_t MessageMaster::TransferEmit(ConfigSend & config_send) {
  if (config_send.data_size != TransferWriteSize()) {
    // Size mismatch, slave will dislike this
    return -1;
  }

  transfer_item.bytes_sent += config_send.data_size;

  return bus_i2c->write(config_send.subsystem, (const char *)config_send.data,
      config_send.data_size);
}

/**
 * @brief Ends a binary transfer
 * TODO: Need to look into how we end a binary transfer. At the very least we
 * should ReadAddress the slave for an ACK that everything went okay.
 *
 * @return mbed_error_status_t
 */
mbed_error_status_t MessageMaster::TransferEnd() {
  if (transfer_item.bytes_sent != transfer_item.protocol.bytes_total) {
    // Bad, don't do this yet.
    return -1;
  }

  return 0;
}
