#include "IFJR.h"
#include "IHU.h"
#include "SPI.h"
#include "mbed.h"
#include "mbed_events.h"
#include "transfer.h"
#include <limits>

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
        IHU::startQueueThread(&ihu);
        ihu.sfh->init();
        SPI spi(PB_15, PB_14, PB_13, PB_12);
        spi.lock();

        init_spi_connection(spi);

        std::fstream file = ihu.sfh->read("firmware.bin",
                                          std::fstream::in | std::fstream::out
                                                  | std::fstream::binary);
        transfer_file(file, spi);
        spi.unlock();
        IHU::clearIHUObject(ihu);
}
