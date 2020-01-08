#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "tests/test_PacketParser.cpp"

int main(int argc, char** argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}