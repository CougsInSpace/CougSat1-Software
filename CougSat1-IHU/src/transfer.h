#ifndef TRANSFER_H
#define TRANSFER_H

#include "mbed.h"
#include <cmath>
#include <fstream>

#define SPI_BIT_WIDTH 8
#define SPI_MODE 0

#define SPI_FREQUENCY 1000000
#define SPI_TEST_MESSAGE 0xFF
#define SPI_ACK 0xAF

int get_num_of_transfers(std::fstream &file);

void init_spi_connection(SPI &spi);

int test_spi_connection(SPI &spi);

int transfer_file(std::fstream &file, SPI &spi);

#endif // TRANSFER_H