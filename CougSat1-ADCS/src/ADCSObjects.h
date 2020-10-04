#ifndef ADCS_H
#define ADCS_H

#include "CISBoard/PinNames.h"
#include "Configuration.h"
#include "interfaces/CDH.h"
#include <mbed.h>
#include <rtos.h>

class ADCS {
private:
  Thread monitor;
  Thread cdhRead;
  char   message[4];
  CDH    cdh;

public:
  ADCS();
  void monitorThread();
  void cdhThread();
  void initThread();
};
#endif /* ADCS_H */