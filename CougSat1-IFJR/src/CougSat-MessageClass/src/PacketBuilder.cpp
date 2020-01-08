/******************************************************************************** 
 * Copyright (c) 2019 by Cougs in Space - Washington State University           *
 * Cougs in Space website: cis.vcea.wsu.edu                                     * 
 *                                                                              *
 * This file is a part of flight and/or ground software for Cougs in Space's    *
 * satellites. This file is proprietary and confidential.                       *
 * Unauthorized copying of this file, via any medium is strictly prohibited.    *
 *******************************************************************************/

/**
* @file PacketBuilder.cpp
* @author Darren Cheatham
* @date 28 Dec 2019
* @brief Builds Packets for Sat->Gnd communication
* This class is responsible for building packets for Ground -> CougSat
* communication. It also generates CRC32 hashes.
*/

#include "PacketBuilder.h"

/**
 * @brief Appends bytes into an uint8_t array
 * 
 * @param packet_pos Start in array
 * @param type Type value / pointer
 * @param count Count of type
 */
template <typename T>
void PacketBuilder::ByteAppender(uint8_t*& packet_pos, T* type, size_t count) {
  size_t count_bytes = count * sizeof(T);
  memcpy(packet_pos, type, count_bytes);
  packet_pos += count_bytes;
}