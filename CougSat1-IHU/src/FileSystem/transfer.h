#ifndef TRANSFER_H
#define TRANSFER_H

#include <mbed.h>
#include <iostream>
#include <fstream>

#define TRANSFER_SPI_MOSI D7
#define TRANSFER_SPI_MISO D2
#define TRANSFER_SPI_SCLK D8
#define TRANSFER_SPI_SSEL D4

void initiate_transfer(const std::fstream &file, size_t size, const std::ios::openmode mode);
void transfer_finished();

#endif // TRANSFER_H