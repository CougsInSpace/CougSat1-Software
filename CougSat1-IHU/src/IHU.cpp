#include "IHU.h"

void IHU::initObjects(IHUObjects &objs)
{
        objs.sfh = &SatFileHandler::getInstance();
        objs.queue = mbed_event_queue();
        objs.queueThread = new Thread();
}

void IHU::startQueueThread(IHUObjects *objs)
{
        auto dispatchQueue = [&]() { objs->queue->dispatch_forever(); };
        objs->queueThread->start(dispatchQueue);
}

void IHU::runEventQueue(IHUObjects *objs, int32_t ms)
{
        objs->queue->dispatch(ms);
}

void IHU::startWatchdog(int32_t timeout_ms)
{
        Watchdog::get_instance().start(timeout_ms);
        auto kick = [&]() { Watchdog::get_instance().kick(); };
        mbed_event_queue()->call_every(timeout_ms - 1000, kick);
}

void IHU::clearIHUObject(IHUObjects &objs)
{
        delete objs.queueThread;
}
