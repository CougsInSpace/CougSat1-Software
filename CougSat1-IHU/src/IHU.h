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
        };

        /// Delete pointers inside IHUObjects.
        /// @param objs IHUObjects to clear.
        void clearIHUObject(IHUObjects &objs);

        /// Allocates the IHU objects.
        /// @param objs IHUObjects to allocate.
        void initObjects(IHUObjects &objs);

        /// Starts the event queue.
        /// @param objs Contains the thread to start.
        void startQueueThread(IHUObjects *objs);

        /// Does the same as startQueueThread, but without the thread. This
        /// function *will* block.
        /// @param objs IHUObjects to use in function.
        /// @param ms Time in milliseconds to run. -1 == run forever.
        void runEventQueue(IHUObjects *objs, int32_t ms = -1);

        /// Adds a watchdog kick function to the events queue to be executed in
        /// timeout_ms.
        /// @param timeout_ms How long to wait before kicking the watchdog.
        /// The max is 32760.
        void startWatchdog(int32_t timeout_ms = 32760);

        /// Adds a one off event to the queue.
        /// @param objs IHUObjects to use for the function call.
        /// @param f Function pointer of function to call.
        /// @param args Variadic arguments to be forwarded to the call function.
        /// @return ID of event in queue.
        template <typename F, typename... Args>
        int addEvent(IHUObjects &objs, F f, Args &&... args)
        {
                return objs.queue->call(f, std::forward<Args>(args)...);
        }

        /// Adds a one off event to the queue.
        /// @param objs IHUObjects to use for the function call.
        /// @param obj The object who's function you want to call.
        /// @param method Function pointer of function to call.
        /// @param args Variadic arguments to be forwarded to the call function.
        /// @return ID of event in queue.
        template <typename T, typename U, typename... Args>
        int addEvent(IHUObjects &objs, T *obj, U (T::*method)(Args...),
                     Args &&... args)
        {
                return objs.queue->call(obj, method,
                                        std::forward<Args>(args)...);
        }

        /// Adds a recurring event to the queue.
        /// @param objs IHUObjects to use for the function call.
        /// @param f Function pointer of function to call.
        /// @param args Variadic arguments to be forwarded to the call function.
        /// @param ms Number of milliseconds to wait before executing function
        /// call.
        /// @return ID of event in queue.
        template <typename F, typename... Args>
        int addEventRecurring(int32_t ms, IHUObjects &objs, F f,
                              Args &&... args)
        {
                return objs.queue->call_every(ms, f,
                                              std::forward<Args>(args)...);
        }

        /// Adds a one off event to the queue.
        /// @param objs IHUObjects to use for the function call.
        /// @param obj The object who's function you want to call.
        /// @param method Function pointer of function to call.
        /// @param args Variadic arguments to be forwarded to the call function.
        /// @return ID of event in queue.
        template <typename T, typename U, typename... Args>
        int addEventRecurring(int32_t ms, IHUObjects &objs, T *obj,
                              U (T::*method)(Args...), Args &&... args)
        {
                return objs.queue->call_every(ms, obj, method,
                                              std::forward<Args>(args)...);
        }

} // namespace IHU

#endif // IHU_H