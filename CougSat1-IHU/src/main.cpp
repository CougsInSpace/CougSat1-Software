#include "IHU.h"
#include "mbed.h"
#include "mbed_events.h"

EventQueue *eventQueue;
Serial pc(SERIAL_TX, SERIAL_RX);

void foo()
{
        pc.printf("LED1!\r\n");
}

void bar()
{
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
        IHU::IHUObjects ihu;
        IHU::startWatchdog();
        IHU::initObjects(ihu);
        IHU::addEvent(ihu, foo);
        ThisThread::sleep_for(2);
        auto breakDispatch = [&]() { ihu.queue->break_dispatch(); };
        IHU::addEvent(ihu, breakDispatch);
        IHU::startQueueThread(&ihu);
        // MOSI(DI), MISO(DO), SCLK(SCK), ChipSelect(CS), CardDetect(CD), crc,
        // debug
        // SatFileHandler testfs(D11, D12, D13, D10, D2, true, true);
        // testfs.init();
        // string testString = "Hello World Big Brain\r\n";
        // /*int a = testfs.sd.init();
        // testfs.fs.format(&sd); //Uncomment if this is ghe first time running
        // this testfs.fs.mount(&sd); pc.printf("%X \r\n", a);*/
        // /* Checks for existing text file. This will be successful on a second
        //    run but not the first run. The file system must be mounted and the
        //    file must be written first */

        // pc.printf("Start write test.\r\n");
        // testfs.write("fuckThisTest", testString);

        // pc.printf("Start append test.\r\n");
        // testString = "this is appended\r\n";
        // testfs.write("fuckThisTest", testString);

        // pc.printf("Start queue test.\r\n");
        // int n = 0;
        // for (int i = 0; i < 5; i += 1) {
        //         std::pair<string, string> p("test1", "fuckthistest" + n);
        //         // testfs.enqueueMessage(p);
        // }
        // testfs.writeStart();

        // pc.printf("Start read test.\r\n");
        // testfs.read(std::string("fuckThisTest.txt"));

        // std::stringstream stream;
        // stream << "Hello!" << std::endl;

        // testfs.write("hello.txt", stream);
        // pc.printf("Write stream test\r\n");
        // mbed_stats_heap_t heap_stats;
        // mbed_stats_heap_get(&heap_stats);
        // printf("Heap size: %lu / %lu bytes\r\n", heap_stats.current_size,
        //        heap_stats.reserved_size);
        // int cnt = osThreadGetCount();
        // mbed_stats_stack_t *stats = (mbed_stats_stack_t *)malloc(
        //         cnt * sizeof(mbed_stats_stack_t));

        // cnt = mbed_stats_stack_get_each(stats, cnt);
        // for (int i = 0; i < cnt; i++) {
        //         printf("Thread: 0x%lX, Stack size: %lu / %lu\r\n",
        //                stats[i].thread_id, stats[i].max_size,
        //                stats[i].reserved_size);
        // }
        // free(stats);

        // timer();

        // pc.printf("Start test check.\r\n");
        // testfs.check();
        // eventQueue->dispatch(10000);
}
