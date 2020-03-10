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

void IHU::deleteIHUObjects(IHUObjects &objs)
{
        delete objs.sfh;
        objs.queueThread->join();
        delete objs.queueThread;
}

void IHU::runEventQueue(IHUObjects *objs)
{
        while (true) {
                objs->queue->dispatch();
        }
}