#ifndef ADCS_H
#define ADCS_H

#include "Configuration.h"
#include "interfaces/CDH.h"
#include <mbed.h>
#include <rtos.h>


class ADCS {
private:
  Thread monitor;
  Thread cdhRead;
  char   message[9];
  CDH    cdh;
  void   cdhThread();

public:
  ADCS();
  void startThread();
};
#endif /* ADCS_H */