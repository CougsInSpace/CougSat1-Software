#ifndef IHU_H
#define IHU_H
#include "mbed.h"

class SatFileHandler;
class IHU
{
    private:
        /// This will point at the mbed_event_queue which is available device
        /// wide.
        static EventQueue *eventQueue;

        /// The thread the event loop will execute in.
        static Thread *eventThread;

        /// Event Data
        ///
        /// Stores a function pointer and a time in milliseconds.
        struct Event {
                void (*func)();
                int ms = -1;
        };

    public:
        /// Add an event to eventQueue.
        /// @param event A reference to the event to push to the queue;
        static void addtoQueue(Event &event);

        /// Initialise the IHU.
        static void init();

        static void run();

        /// Static pointer to the File Handler. SatFileHandler cannot be copied.
        /// This is public to facilitate simple usage of the filesystem.
        static SatFileHandler *sfh;

        IHU();
        ~IHU();
};

#endif // IHU_H