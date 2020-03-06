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
        SatFileHandler testfs(D11, D12, D13, D10, 1000000, false, true);
        testfs.init();
        pc.printf("Start file write test \r\n");
        string testString("Hello World Big Brain\r\n");
        /*int a = testfs.sd.init();
        testfs.fs.format(&sd); //Uncomment if this is ghe first time running
        this testfs.fs.mount(&sd); pc.printf("%X \r\n", a);*/
        /* Checks for existing text file. This will be successful on a second
           run but not the first run. The file system must be mounted and the
           file must be written first */

        if (testfs.write(std::string("fuckThisTest"), testString)) {
                pc.printf("Halfway there WHOA\r\n");

                testString = "this is appended\r\n";
                if (testfs.write(std::string("fuckThisTest"), testString)) {
                        pc.printf("LIVIN ON A PRAYER\r\n");
                }
        }

        else
                pc.printf("Fail \r\n");

        pc.printf("Start 2, enqueue test \r\n");
        int n = 0;
        for (int i = 0; i < 5; i += 1) {
                std::pair<string, string> p("test1", "fuckthistest" + n);
                testfs.enqueueMessage(p);
        }

        if (testfs.writeStart()) {
                pc.printf("Success 2 \r\n");
        } else {
                pc.printf("Fail 2 \r\n");
        }

        pc.printf("Start 3, read test \n\r");
        if (testfs.read(std::string("fuckThisTest.txt")) != "") {
                pc.printf("Success 3 \r\n");
        } else {
                pc.printf("Fail 3 \r\n");
        }

        // pc.printf("Start 4, check test \n\r");
        // if (testfs.check()) {
        //         pc.printf("Success 4 \r\n");
        // } else {
        //         pc.printf("Fail 4 \r\n");
        // }
}