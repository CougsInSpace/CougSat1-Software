#include "IHU.h"

void IHU::initObjects(IHUObjects &objs)
{
        objs.sfh = new SatFileHandler(D11, D12, D13, D10, D2, false, true);
        objs.queue = mbed_event_queue();
        objs.queueThread = new Thread();
}

void IHU::startQueueThread(IHUObjects *objs)
{
        objs->queueThread->start(callback(runEventQueue, objs));
}

void IHU::runEventQueue(IHUObjects *objs)
{
        while (true) {
                objs->queue->dispatch();
        }
}

void IHU::startWatchdog(int timeout_ms)
{
        auto kick = [&]() { Watchdog::get_instance().kick(); };
        mbed_event_queue()->call_every(timeout_ms, kick);
}

void IHU::clearIHUObject(IHUObjects &objs)
{
        delete objs.sfh;
        delete objs.queueThread;
}