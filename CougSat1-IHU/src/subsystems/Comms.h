#ifndef SRC_SUBSYSTEMS_COMMS_H_
#define SRC_SUBSYSTEMS_COMMS_H_

#include "Subsystem.h"

#include <mbed.h>

class Comms : public Subsystem {
public:
  Comms(const Comms &) = delete;
  Comms & operator=(const Comms &) = delete;

  Comms(SPI & spi);
  ~Comms();

  mbed_error_status_t initialize();

private:
  SPI & spi;
};

#endif /* SRC_SUBSYSTEMS_COMMS_H_ */