#ifndef ADCS_H
#define ADCS_H

#include <mbed.h>
#include <rtos.h>
#include "Configuration.h"
#include "interfaces/CDH.h"
#include "CISBoard/PinNames.h"

class ADCS
{
  private:
	Thread monitor;
	Thread cdhRead;
	char message[4];
    #define IHU_ADDRESS_TEST 0xAC
	CDH cdh;
	
  public:
	ADCS();
	void monitorThread();
	void cdhThread();
	void initThread();
};
#endif /* ADCS_H */