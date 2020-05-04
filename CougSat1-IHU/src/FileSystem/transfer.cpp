#include "transfer.h"

static SPI *spi = nullptr;


void initiate_transfer(const std::fstream &file, size_t size, const std::ios::openmode mode)
{
        spi = new SPI(TRANSFER_SPI_MOSI, TRANSFER_SPI_MISO,
                           TRANSFER_SPI_SCLK, TRANSFER_SPI_SSEL);
        std::fstream out;
        out.copyfmt(file);
        out << file.rdbuf();
        spi->transfer<std::fstream>(&out, size, nullptr, 0, (event_callback_t&)transfer_finished);
}

void transfer_finished()
{
        Serial pc(SERIAL_TX, SERIAL_RX);
        pc.printf("Transfer finished\r\n");
        delete spi;
}