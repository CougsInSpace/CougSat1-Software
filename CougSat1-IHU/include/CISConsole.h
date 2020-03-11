#ifndef SRC_TOOLS_CISCONSOLE_H_
#define SRC_TOOLS_CISCONSOLE_H_

#include <SWO.h>
#include <mbed.h>

// #define NDEBUG
// #define NLOG
// #define NERROR

class SWOSingleton {
public:
  SWOSingleton(const SWOSingleton &) = delete;
  SWOSingleton & operator=(const SWOSingleton &) = delete;

  /**
   * @brief Get the singleton object
   *
   * @return SWOSingleton*
   */
  static SWOSingleton * Instance() {
    static SWOSingleton instance;
    return &instance;
  }

  /**
   * @brief Destroy the SWOSingleton object
   *
   */
  ~SWOSingleton() {}

  /**
   * @brief Get the SWO channel
   *
   * @return SWO_Channel*
   */
  SWO_Channel * get() {
    return swo;
  }

private:
  /**
   * @brief Construct a new SWOSingleton object
   *
   */
  SWOSingleton() {
    swo = new SWO_Channel();
  }

  SWO_Channel * swo;
};

/**
 * Prints to the SWO port as follows:
 * "[time stamp] object name: message"
 * @param o name of class or function
 * @param args... standard printf arguments
 */
#ifndef NDEBUG
#define DEBUG(o, args...)                                                      \
  {                                                                            \
    SWO_Channel * swo = SWOSingleton::Instance()->get();                       \
    swo->printf("[%07lu][Debug] %-10s: ", HAL_GetTick(), o);                   \
    swo->printf(args);                                                         \
    swo->putc('\n');                                                           \
  }
#else
#define DEBUG(o, ...)                                                          \
  {}
#endif

/**
 * Prints to the SWO port as follows:
 * "[time stamp] object name: message"
 * @param o name of class or function
 * @param args... standard printf arguments
 */
#ifndef NLOG
#define LOG(o, args...)                                                        \
  {                                                                            \
    SWO_Channel * swo = SWOSingleton::Instance()->get();                       \
    swo->printf("[%07lu][ Log ] %-10s: ", HAL_GetTick(), o);                   \
    swo->printf(args);                                                         \
    swo->putc('\n');                                                           \
  }
#else
#define LOG(o, ...)                                                            \
  {}
#endif

/**
 * Prints to the SWO port as follows:
 * "[time stamp] object name: message"
 * @param o name of class or function
 * @param args... standard printf arguments
 */
#ifndef NERROR
#define ERROR(o, args...)                                                      \
  {                                                                            \
    SWO_Channel * swo = SWOSingleton::Instance()->get();                       \
    swo->printf("[%07lu][Error] %-10s: ", HAL_GetTick(), o);                   \
    swo->printf(args);                                                         \
    swo->putc('\n');                                                           \
  }
#else
#define ERROR(o, ...)                                                          \
  {}
#endif

#endif /* SRC_TOOLS_CISCONSOLE_H_ */
