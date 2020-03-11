#ifndef IHU_H
#define IHU_H
#include "FileSystem/SatFileHandler.h"
#include "mbed.h"

/// Namespace containing functions for the main IHU control loop.
namespace IHU
{
        /// Holds relevant IHU objects.
        struct IHUObjects {
                /// File handler for IHU(SD card).
                SatFileHandler *sfh;

                /// Points to the shared event queue.
                EventQueue *queue;

                /// This is where events are dispatched.
                Thread *queueThread;

                ~IHUObjects();
        };

        /// Allocates the IHU objects.
        /// @param objs IHUObjects to allocate.
        void initObjects(IHUObjects &objs);

        /// Starts the event queue.
        /// @param objs Contains the thread to start.
        void startQueueThread(IHUObjects *objs);

        /// Does the same as startQueueThread, but without the thread. This
        /// function *will* block.
        /// @param objs IHUObjects to use in function.
        void runEventQueue(IHUObjects *objs);

        /// Adds a watchdog kick function to the events queue to be executed in
        /// timeout_ms.
        /// @param timeout_ms How long to wait before kicking the watchdog.
        /// TODO: Implement.
        void startWatchdog(int timeout_ms = 300'000);

        /// Adds a one off event to the queue.
        /// @param objs IHUObjects to use for the function call.
        /// @param func Function pointer of function to call. NOTE: This will
        /// only work with functions that *are not* a part of another object.
        /// @param args Variatic arguments to be forwarded to the call function.
        /// @return ID of event in queue.
        template <typename... Args>
        int addEvent(IHUObjects &objs, void (*func)(), Args &&... args)
        {
                return objs.queue->call(func, std::forward<Args>(args)...);
        }

        /// Adds a one off event to the queue.
        /// @param objs IHUObjects to use for the function call.
        /// @param targObj Object that the function pointer belongs to.
        /// @param func Function pointer of function to call. NOTE: This will
        /// only work with functions that *are not* a part of another object.
        /// @param args Variatic arguments to be forwarded to the call function.
        /// @return ID of event in queue.
        template <typename T, typename... Args>
        int addEvent(IHUObjects &objs, T *targObj, void (*func)(),
                     Args &&... args)
        {
                return objs.queue->call(&targObj, func,
                                        std::forward<Args>(args)...);
        }

        /// Adds a recurring event to the queue.
        /// @param objs IHUObjects to use for the function call.
        /// @param func Function pointer of function to call. NOTE: This will
        /// only work with functions that *are not* a part of another object.
        /// @param args Variatic arguments to be forwarded to the call function.
        /// @param ms Number of milliseconds to wait before executing function
        /// call.
        /// @return ID of event in queue.
        template <typename... Args>
        int addEventRecurring(IHUObjects &objs, void (*func)(), Args &&... args,
                              int ms)
        {
                return objs.queue->call_every(ms, func,
                                              std::forward<Args>(args)...);
        }

        /// Adds a recurring event to the queue.
        /// @param objs IHUObjects to use for the function call.
        /// @param targObj Object that the function pointer belongs to.
        /// @param func Function pointer of function to call. NOTE: This will
        /// only work with functions that *are not* a part of another object.
        /// @param args Variatic arguments to be forwarded to the call function.
        /// @param ms Number of milliseconds to wait before executing function
        /// call.
        /// @return ID of event in queue.
        template <typename T, typename... Args>
        int addEventRecurring(IHUObjects &objs, T *targObj, void (*func)(),
                              Args &&... args, int ms)
        {
                return objs.queue->call_every(ms, &targObj, func,
                                              std::forward<Args>(args)...);
        }
} // namespace IHU

#endif // IHU_H