#ifndef SRC_SUBSYSTEMS_PAYLOAD_H_
#define SRC_SUBSYSTEMS_PAYLOAD_H_

#include "Subsystem.h"

#include <mbed.h>

class Payload : public Subsystem {
public:
  Payload(const Payload &) = delete;
  Payload & operator=(const Payload &) = delete;

  Payload();
  ~Payload();

  mbed_error_status_t initialize();

private:
};

#endif /* SRC_SUBSYSTEMS_PAYLOAD_H_ */