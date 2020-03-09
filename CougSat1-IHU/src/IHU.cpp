#include "IHU.h"
#include "SatFileHandler.h"

SatFileHandler *IHU::sfh = nullptr;
EventQueue *IHU::eventQueue = mbed_event_queue();
Thread *IHU::eventThread = nullptr;

IHU::IHU()
{
}

IHU::~IHU()
{
        delete eventQueue;
        delete sfh;
        delete eventThread;
}

void IHU::addtoQueue(Event &event)
{
        eventQueue->call_every(event.ms, event.func);
}