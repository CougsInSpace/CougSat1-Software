// #include "HeapBlockDevice.h"
#include "SatFileHandler.h"
#include "mbed.h"
#include <FATFileSystem.h>
#include <cstdio>

// SDBlockDevice sd(D11, D12, D13, D10);
// FATFileSystem fs("sd");

// HeapBlockDevice sd(2048, 256);

// All tests passed 03/06/20
int main()
{
        Serial pc(SERIAL_TX, SERIAL_RX);
        // MOSI(DI), MISO(DO), SCLK(SCK), ChipSelect(CS), CardDetect(CD), crc,
        // debug
        SatFileHandler testfs(D11, D12, D13, D10, D2, true, true);
        testfs.init();
        string testString = "Hello World Big Brain\r\n";
        /*int a = testfs.sd.init();
        testfs.fs.format(&sd); //Uncomment if this is ghe first time running
        this testfs.fs.mount(&sd); pc.printf("%X \r\n", a);*/
        /* Checks for existing text file. This will be successful on a second
           run but not the first run. The file system must be mounted and the
           file must be written first */

        pc.printf("Start write test.\r\n");
        testfs.write("fuckThisTest", testString);

        pc.printf("Start append test.\r\n");
        testString = "this is appended\r\n";
        testfs.write("fuckThisTest", testString);

        pc.printf("Start queue test.\r\n");
        int n = 0;
        for (int i = 0; i < 5; i += 1) {
                std::pair<string, string> p("test1", "fuckthistest" + n);
                testfs.enqueueMessage(p);
        }
        testfs.writeStart();

        pc.printf("Start read test.\r\n");
        testfs.read(std::string("fuckThisTest.txt"));

        pc.printf("Start test check.\r\n");
        testfs.check();
}