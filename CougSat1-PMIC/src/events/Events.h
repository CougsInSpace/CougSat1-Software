#ifndef _SRC_EVENTS_EVENTS_H_
#define _SRC_EVENTS_EVENTS_H_

#include <mbed.h>

mbed_error_status_t eventPOST();
mbed_error_status_t eventPeriodic();
mbed_error_status_t eventFirstBoot();
mbed_error_status_t eventDeploy(int i, bool force);

#endif /* _SRC_EVENTS_EVENTS_H_ */