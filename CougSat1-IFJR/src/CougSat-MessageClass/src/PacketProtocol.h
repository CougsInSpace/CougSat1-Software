/******************************************************************************** 
 * Copyright (c) 2019 by Cougs in Space - Washington State University           *
 * Cougs in Space website: cis.vcea.wsu.edu                                     * 
 *                                                                              *
 * This file is a part of flight and/or ground software for Cougs in Space's    *
 * satellites. This file is proprietary and confidential.                       *
 * Unauthorized copying of this file, via any medium is strictly prohibited.    *
 *******************************************************************************/

/**
* @file PacketProtocol.hpp
* @author Darren Cheatham
* @date 19 Dec 2019
* @brief Contains common data structures for Packet Protocol
*/

#pragma once

#include <stdlib.h>
#include <iostream>
#include <tuple>

namespace PacketProtocol {
  /**
   * @brief Sender of Command
   */
  enum Sender {
    Ground = 0x0,
    CougSat1
  };

  /**
   * @brief Recipient of Command
   */
  enum Recipient {
    ADCS = 0x0,
    IFJR,
    IHU,
    PMIC,
    Comms,
    Payload1,     //Health Data Logging
    Payload2,     //Ground Facing Camera
    Payload3      //Germination Experiment
  };

  /**
   * @brief Struct of Packet Header
   */
  struct Header {
    uint8_t command;
    Recipient recipient;
    Sender sender;
  };

  /**
   * @brief Struct of MultiPacket Header
   */
  struct MultiPacketHeader {
    uint16_t serial;
    uint32_t file_size;
    uint8_t file_crc[5];
    std::string file_name;
  };

  /**
   * @brief Commands Namespace
   */
  namespace Commands {
    /**
     * @brief All IFJR commands
     */
    enum IFJR {
      ProcessorBinaryImageUpload = 0,
      ReprogramDevice
    };

    /**
     * @brief All IHU Commands
     */
    enum IHUs {
      Telemetry = 0
    };

    /**
     * @brief All Comms Commands
     */
    enum Comms {
      RadioConfiguration = 0
    };

    /**
     * @brief All Payload Commands
     */
    enum Payload {
      Configuration = 0,
      DataDownload
    };

    /**
     * @brief List of commands that require multipackets
     * TODO: Find a shothand way to write this
     */
    const std::tuple<Sender, Recipient, uint8_t> MultiPackets[] = {
      std::make_tuple<Sender, Recipient, uint8_t>(Sender::Ground, Recipient::IFJR, IFJR::ProcessorBinaryImageUpload),

      //Payloads get configuration
      std::make_tuple<Sender, Recipient, uint8_t>(Sender::Ground, Recipient::Payload1, Payload::Configuration),
      std::make_tuple<Sender, Recipient, uint8_t>(Sender::Ground, Recipient::Payload2, Payload::Configuration),
      std::make_tuple<Sender, Recipient, uint8_t>(Sender::Ground, Recipient::Payload3, Payload::Configuration),

      //Payloads data download
      std::make_tuple<Sender, Recipient, uint8_t>(Sender::CougSat1, Recipient::Payload1, Payload::DataDownload),
      std::make_tuple<Sender, Recipient, uint8_t>(Sender::CougSat1, Recipient::Payload2, Payload::DataDownload),
      std::make_tuple<Sender, Recipient, uint8_t>(Sender::CougSat1, Recipient::Payload3, Payload::DataDownload),
    };
  }

  const uint16_t PACKET_MAX_SIZE = 1024;
  const uint16_t PACKET_MAX_USEABLE = 1022;
  const uint16_t MULTI_PACKET_MAX_USEABLE = 1020;
}