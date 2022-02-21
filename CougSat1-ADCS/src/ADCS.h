#ifndef ADCS_H
#define ADCS_H

#include "Configuration.h"
#include "interfaces/CDH.h"
#include <mbed.h>
#include <rtos.h>
#include <../../eigen/Eigen/Dense>


class ADCS {
private:
  Thread monitor;
  Thread cdhRead;
  Thread attitudeDeterminationThread;
  char   message[9];
  CDH    cdh;
  void   cdhThread();

public:
  ADCS();
  void startThread();
  void attitudeDetermination();
};
#endif /* ADCS_H */