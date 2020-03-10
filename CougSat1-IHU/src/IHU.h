#ifndef IHU_H
#define IHU_H
#include "FileSystem/SatFileHandler.h"
#include "mbed.h"

namespace IHU
{
        struct IHUObjects {
                SatFileHandler *sfh;
                EventQueue *queue;
                Thread *queueThread;
        };

        void initObjects(IHUObjects &objs);

        void startQueueThread(IHUObjects *objs);

        void deleteIHUObjects(IHUObjects &objs);

        void runEventQueue(IHUObjects *objs);

        template <typename... Args>
        inline void addEvent(IHUObjects &objs, void (*func)(), Args &&... args)
        {
                objs.queue->call(func, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inline void addEvent(IHUObjects &objs, void (*func)(), Args &&... args,
                             int ms)
        {
                objs.queue->call(ms, func, std::forward<Args>(args)...);
        }
} // namespace IHU

#endif // IHU_H