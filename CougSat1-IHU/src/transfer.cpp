#include "transfer.h"

int get_num_of_transfers(std::fstream &file)
{
        file.ignore(std::numeric_limits<std::streamsize>::max());
        std::streamsize length = file.gcount();
        file.clear(); //  Since ignore will have set eof.
        file.seekg(0, std::ios_base::beg);
        return length / SPI_BIT_WIDTH + (length % SPI_BIT_WIDTH != 0);
}

void init_spi_connection(SPI &spi)
{
        spi.frequency(SPI_FREQUENCY);
        spi.format(SPI_BIT_WIDTH, SPI_MODE);
}

int test_spi_connection(SPI &spi)
{
        spi.write(SPI_TEST_MESSAGE);
        int response = spi.write(0x00);
        if (response == SPI_ACK)
                return 0;
        else
                return -1;
}

int transfer_file(std::fstream &file, SPI &spi)
{
        int transfers = get_num_of_transfers(file);
        char byte = 0;

        auto send_byte = [&](int packet_num) {
                file.read(&byte, 1);
                spi.write((int)byte);
        };
        while (spi.write(0x00) != SPI_ACK)
                ;
        for (int i = 0; i < transfers; i++) {
                send_byte(i);
        }
}