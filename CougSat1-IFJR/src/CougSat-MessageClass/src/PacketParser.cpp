/******************************************************************************** 
 * Copyright (c) 2019 by Cougs in Space - Washington State University           *
 * Cougs in Space website: cis.vcea.wsu.edu                                     * 
 *                                                                              *
 * This file is a part of flight and/or ground software for Cougs in Space's    *
 * satellites. This file is proprietary and confidential.                       *
 * Unauthorized copying of this file, via any medium is strictly prohibited.    *
 *******************************************************************************/

/**
* @file PacketParser.cpp
* @author Darren Cheatham
* @date 19 Dec 2019
* @brief Parses Packets for Gnd->Sat communication
* This class is responsible for parsing packets for Ground -> CougSat
* communication. It also validate CRC hashes.
*/

#include "PacketParser.h"

/**
 * @brief Slides bytes out of an uint8_t array
 * It works by incrementing packet_pos every time it adds data
 * 
 * @param type Type value / pointer
 * @param packet_pos Start in array
 * @param count Count of type (Up to 255, please adjust if necessary)
 */
template <typename T>
void PacketParser::ByteSlider(T* destination, uint8_t*& packet_pos, size_t count) {
  size_t count_bytes = count * sizeof(T);
  for (uint8_t pos = 0; pos < count; pos++) {
    //Because of integer math, we have to account for the size of T:
    memcpy(destination + pos, packet_pos + (pos * sizeof(T)), count_bytes);
  }
  packet_pos += count_bytes;
}

/**
 * @brief Parses the first byte of a packet header
 * 
 * @param header 
 * @return PacketProtocol::Header 
 */
PacketProtocol::Header PacketParser::ParseHeader(uint8_t header) {
  return {
    static_cast<uint8_t>((header & 0b11000000) >> 6),
    (PacketProtocol::Recipient)((0b00111000 & header) >> 3),
    (PacketProtocol::Sender)(0b00000111 & header)
  };
}

/**
 * @brief Parses a multipacket header
 * 
 * Byte 0-1:  Serial number
 * Byte 2-5:  File size
 * Byte 6-10: CRC32
 * Byte 11-?: File name
 * 
 * @param multi_header Start of multipacket header
 * @return PacketProtocol::MultiPacketHeader 
 */
PacketProtocol::MultiPacketHeader PacketParser::ParseMultiPacketHeader(uint8_t* multi_header) {
  PacketProtocol::MultiPacketHeader data;

  PacketParser::ByteSlider<uint16_t>(&data.serial, multi_header);
  PacketParser::ByteSlider<uint32_t>(&data.file_size, multi_header);
  PacketParser::ByteSlider<uint8_t>(data.file_crc, multi_header, 5);
  data.file_name = std::string((char*)(multi_header));

  return data;
}

/**
 * @brief Construct a new Packet Parser:: Packet Parser object
 * 
 * @param bytes 
 * @param count 
 */
PacketParser::PacketParser(uint8_t* bytes, size_t count) {
  this->header = PacketParser::ParseHeader(*bytes);

  if (this->IsMultiPacket()) {
    this->multipacket_header = PacketParser::ParseMultiPacketHeader(bytes + 2);
    this->total_packets = (this->multipacket_header.file_size  / PacketProtocol::MULTI_PACKET_MAX_USEABLE);
  }

  this->current_bytes = count;
  this->total_bytes = (*(bytes + 1) + 1) * 4;
}

/**
 * @brief Gets parsed packet header
 * 
 * @return PacketProtocol::Header 
 */
PacketProtocol::Header PacketParser::GetHeader() {
  return this->header;
}

/**
 * @brief Gets parsed multipacket header
 * 
 * @return PacketProtocol::MultiPacketHeader 
 */
PacketProtocol::MultiPacketHeader PacketParser::GetMultiPacketHeader() {
  return this->multipacket_header;
}

/**
 * @brief Gets message total packets
 * 
 * @return size_t 
 */
size_t PacketParser::GetTotalPacketsCount() {
  return this->total_packets;
}

/**
 * @brief Gets message total bytes
 * 
 * @return size_t 
 */
size_t PacketParser::GetTotalBytesCount() {
  return this->total_bytes;
}

/**
 * @brief Gets message current packets
 * 
 * @return size_t 
 */
size_t PacketParser::GetCurrentPacketsCount() {
  return this->current_packets;
}

/**
 * @brief Gets message current bytes
 * 
 * @return size_t 
 */
size_t PacketParser::GetCurrentBytesCount() {
  return this->current_bytes;
}

/**
 * @brief Determines if the current packet is a MultiPacket
 * 
 * @return true 
 * @return false 
 */
bool PacketParser::IsMultiPacket() {
  for (auto command : PacketProtocol::Commands::MultiPackets) {
    if (
      std::get<0>(command) == this->header.command &&
      std::get<1>(command) == this->header.recipient &&
      std::get<2>(command) == this->header.sender
    ) {
      return true;
    }
  }

  return false;
}