#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sodium.h>
#include <vector>

#include "PacketParser.h"
#include "test_Helpers.h"

//if different command is mixed in, deal with it differently
//make sure large data transfers use minimal memory
//test when packet header goes missing but data is still sent
//Test missing packets

using namespace testing;


/**
 * @brief Test PacketParser's header parsing
 * Bit 0-1: Command ID
 * Bit 2-4: Recipient
 * Bit 5-7: Sender
 */
TEST(PacketParser, ParseHeaderFirstByte) {
  //Command 3, Recipient Payload1, Sender CougSat1
  uint8_t packet[] = { 0b11101001, 0, 0, 0 };

  auto parser = PacketParser(packet, 4);
  auto header = parser.GetHeader();

  ASSERT_EQ(
    PacketProtocol::Sender::CougSat1,
    header.sender
  );

  ASSERT_EQ(
    PacketProtocol::Recipient::Payload1,
    header.recipient
  );

  ASSERT_EQ(3, header.command);
}

/**
 * @brief Test PacketParser's packet length parsing
 * Byte 0: Header
 * Byte 1: Packet Length
 * Byte 2: Padding
 * Byte 3: Padding
 */
TEST(PacketParser, ParseHeaderSecondByte) {
  //Min edge case
  uint8_t min_packet[] = { 0, 0, 0, 0 };
  auto min = PacketParser(min_packet, 4);
  ASSERT_EQ(1, min.GetTotalPacketsCount());
  ASSERT_EQ(4, min.GetTotalBytesCount());

  //Max edge case
  uint8_t max_packet[] = { 0, UINT8_MAX, 0, 0 };
  auto max = PacketParser(max_packet, 4);
  ASSERT_EQ(1, max.GetTotalPacketsCount());
  ASSERT_EQ(1024, max.GetTotalBytesCount());
}

/**
 * @brief Test Packet Parser's MultiPacket infomation parsing
 * 
 * Byte 0-1:  Serial number
 * Byte 2-5:  File length
 * Byte 6-10: CRC32
 * Byte 11-?: File name
 */
TEST(PacketParser, ParseMultiPacketHeader) {
  uint8_t packet[1024] = { 0 }; //Max single packet length
  uint8_t header[] = { 0b00001000, 0 }; //Ground -> IFJR, Binary Image Upload
  uint32_t file_size = 800 / 4 - 1; //File size in dwords

  uint8_t file_crc[5];
  randombytes_buf(file_crc, 5);

  char file_name[] = "TestFile.dat";

  uint16_t serial = randombytes_random();


  //Add some random bytes as payload
  uint8_t data[800];
  randombytes_buf(data, 800);

  //Assemble the packet
  uint8_t* packet_pos = packet;
  TEST_MemcpyHelper<uint8_t>(packet_pos, &header[0], 2);
  TEST_MemcpyHelper<uint16_t>(packet_pos, &serial, 1);
  TEST_MemcpyHelper<uint32_t>(packet_pos, &file_size, 1);
  TEST_MemcpyHelper<uint8_t>(packet_pos, &file_crc[0], 5);
  TEST_MemcpyHelper<char>(packet_pos, &file_name[0], strlen(file_name) + 1);
  TEST_MemcpyHelper<uint8_t>(packet_pos, &data[0], 800);

  //Test that the parsing is correct
  auto result = PacketParser(packet, 1024);
  auto multiheader = result.GetMultiPacketHeader();

  ASSERT_EQ(serial, multiheader.serial);
  ASSERT_EQ(file_size, multiheader.file_size);
  ASSERT_EQ(file_name, multiheader.file_name);

  for (int pos = 0; pos < 5; pos++) {
    ASSERT_EQ(*(file_crc + pos), *(multiheader.file_crc + pos));
  }
}

/**
 * @brief Simply parse and return monopacket data
 */
TEST(PackerParser, ParsePacketData) {
  
}