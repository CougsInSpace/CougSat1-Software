#pragma once

#include "CRC.h"

#define key 0x2F

char CRC8(const char * data, int length) {
  char crc = 0x00;
  for (int i = 0; i < length; ++i) {
    crc ^= *(data + i);

    for (int j = 0; j < 8; ++j) {
      if ((crc & 0x80)) {
        crc = ((crc << 1) ^ key);
      } else {
        crc <<= 1;
      }
    }
  }

  return crc;
}