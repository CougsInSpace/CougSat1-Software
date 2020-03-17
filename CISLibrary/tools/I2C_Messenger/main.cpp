/**
 * @file main.cpp
 * @author Darren Cheatham
 * @date 16 March 2020
 * @brief Example usage of MessageMaster
 */

#include "MessageMaster.h"

#include <iostream>
#include <mbed.h>

int main() {
  MessageMaster master(I2C_SDA, I2C_SCL);

  // Try out a send and recv (static size) transfer:
  const int size_send = 23;
  const int size_recv = 54;

  int data_send[size_send] = {};
  int data_recv[size_recv] = {};

  ConfigSend to_send = {
    IFJR,       // Who
    0,          // Command
    size_send,  // How much data
    data_send   // Data
  };

  ConfigRecv to_recv = { size_recv, data_recv };

  int code = master.Send(to_send, to_recv);

  if (code == MBED_SUCCESS) {
    std::cout << "It worked!" << std::endl;
  } else {
    std::cout << "Start crying!" << std::endl;
  }

  // Try out a big item transfer:
  const int bigboi_size         = 2439034;
  int       bigboi[bigboi_size] = {};

  TransferProtocol bigboi_transfer_infomation = {
    bigboi_size * sizeof(int),  // How many bytes
    0                           // CRC32 hash
  };

  ConfigSend bigboi_send = { IFJR, 42, sizeof(TransferProtocol), &bigboi_transfer_infomation };

  master.TransferStart(bigboi_send);

  for (int pos = 0; pos < bigboi_size; pos += (PACKET_MAX_SIZE / sizeof(int)) {
    ConfigSend bigboi_emit = { IFJR, 42, PACKET_MAX_SIZE, bigboi[pos] };
    code                   = master.TransferEmit(bigboi_emit);

    if (code != MBED_SUCCESS) {
      std::cout << "Prepare to cry!" << std::endl;
    }
  }

  code = master.TransferEnd();

  if (code != MBED_SUCCESS) {
    std::cout << "Always cry!" << std::endl;
  }
 

  return 0;
}
