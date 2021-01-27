#include "SatFileHandler.h"
#include <sstream>

bool testFS()
{
        Serial pc(SERIAL_TX, SERIAL_RX);
        // MOSI(DI), MISO(DO), SCLK(SCK), ChipSelect(CS), CardDetect(CD), crc,
        // debug
        SatFileHandler *testfs = &SatFileHandler::getInstance();
        testfs->init();
        string testString = "Hello World Big Brain\r\n";

        pc.printf("Start write test.\r\n");
        testfs->write("fuckThisTest", testString);

        pc.printf("Start append test.\r\n");
        testString = "this is appended\r\n";
        testfs->write("fuckThisTest", testString);

        pc.printf("Start queue test.\r\n");

        pc.printf("Start read test.\r\n");
        testfs->read(std::string("fuckThisTest.txt"));

        std::stringstream stream;
        stream << "Hello!" << std::endl;

        testfs->write("hello.txt", stream);
        pc.printf("Write stream test\r\n");

        pc.printf("Start test check.\r\n");
        testfs->check();
        return true;
}