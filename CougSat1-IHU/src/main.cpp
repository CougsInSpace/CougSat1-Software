// #include "HeapBlockDevice.h"
#include "SatFileHandler.h"
#include "mbed.h"
#include "mbed_events.h"
#include <FATFileSystem.h>
#include <cstdio>
#include <sstream>

EventQueue *eventQueue;
DigitalOut led1(LED1);
DigitalOut led2(LED2);
Serial pc(SERIAL_TX, SERIAL_RX);
void foo()
{
        led1 = !led1;
        pc.printf("LED1!\r\n");
}

void bar()
{
        led2 = !led2;
        pc.printf("LED2!\r\n");
}

void timer()
{
        Timer t1, t2;
        t1.start();
        t2.start();
        while (true) {
                if (t1.read_ms() >= 2500) {
                        eventQueue->call(foo);
                        t1.reset();
                }
                if (t2.read_ms() >= 5000) {
                        eventQueue->call(bar);
                        t2.reset();
                }
                eventQueue->dispatch(1000);
        }
}

// All tests passed 03/06/20
int main()
{
        eventQueue = mbed_event_queue();

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
                // testfs.enqueueMessage(p);
        }
        testfs.writeStart();

        pc.printf("Start read test.\r\n");
        testfs.read(std::string("fuckThisTest.txt"));

        std::stringstream stream;
        stream << "Hello!" << std::endl;

        testfs.write("hello.txt", stream);
        pc.printf("Write stream test\r\n");
        timer();

        // pc.printf("Start test check.\r\n");
        // testfs.check();
        // eventQueue->dispatch(10000);
}