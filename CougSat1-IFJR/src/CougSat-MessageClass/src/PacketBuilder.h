/******************************************************************************** 
 * Copyright (c) 2019 by Cougs in Space - Washington State University           *
 * Cougs in Space website: cis.vcea.wsu.edu                                     * 
 *                                                                              *
 * This file is a part of flight and/or ground software for Cougs in Space's    *
 * satellites. This file is proprietary and confidential.                       *
 * Unauthorized copying of this file, via any medium is strictly prohibited.    *
 *******************************************************************************/

/**
* @file PacketBuilder.h
* @author Darren Cheatham
* @date 28 Dec 2019
* @brief Builds Packets for Sat->Gnd communication
* This class is responsible for building packets for Ground -> CougSat
* communication. It also generates CRC32 hashes.
*/

#pragma once

#include <stdlib.h>

#include "PacketProtocol.h"

class PacketBuilder {
  template <typename T>
  static void ByteAppender(uint8_t*& packet_pos, T* type, size_t count);
public:
};