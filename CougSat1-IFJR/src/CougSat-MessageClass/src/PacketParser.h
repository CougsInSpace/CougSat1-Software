/******************************************************************************** 
 * Copyright (c) 2019 by Cougs in Space - Washington State University           *
 * Cougs in Space website: cis.vcea.wsu.edu                                     * 
 *                                                                              *
 * This file is a part of flight and/or ground software for Cougs in Space's    *
 * satellites. This file is proprietary and confidential.                       *
 * Unauthorized copying of this file, via any medium is strictly prohibited.    *
 *******************************************************************************/

/**
* @file PacketParser.h
* @author Darren Cheatham
* @date 19 Dec 2019
* @brief Parses Packets for Gnd->Sat communication
* This class is responsible for parsing packets for Ground -> CougSat
* communication. It also validates CRC32 hashes.
*/

#include <stdlib.h>
#include <vector>
#include <iostream>

#include "PacketProtocol.h"
#include "PacketBuilder.h"

class PacketParser {
  template <typename T>
  static void ByteSlider(T* destination, uint8_t*& packet_pos, size_t count=1);

  inline static PacketProtocol::Header ParseHeader(uint8_t raw_header);
  inline static PacketProtocol::MultiPacketHeader ParseMultiPacketHeader(uint8_t* header);

  PacketProtocol::Header header;
  PacketProtocol::MultiPacketHeader multipacket_header;

  size_t total_packets = 1;
  size_t total_bytes = 4;

  size_t current_bytes = 1;
  size_t current_packets = 1;
public:
  PacketParser(uint8_t* bytes, size_t count);

  PacketProtocol::Header GetHeader();
  PacketProtocol::MultiPacketHeader GetMultiPacketHeader();

  bool IsMultiPacket();

  size_t GetTotalPacketsCount();
  size_t GetTotalBytesCount();

  size_t GetCurrentPacketsCount();
  size_t GetCurrentBytesCount();

  bool HasAllPackets();
  std::vector<uint16_t> GetMissingSerials();
};