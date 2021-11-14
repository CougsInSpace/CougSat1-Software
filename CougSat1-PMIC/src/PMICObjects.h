#ifndef _SRC_PMIC_OBJECTS_H_
#define _SRC_PMIC_OBJECTS_H_

#include "Configuration.h"
#include "interfaces/CDH.h"

#include <ADC/ADC.h>
#include <CurrentNode.h>
#include <Thermistor.h>
#include <VoltageNode.h>
#include <mbed.h>

/*************************** Interfaces and Drivers ***************************/
extern I2C      i2cLocal;
extern I2CSlave i2cBus;

extern CDH cdh;

extern ADC * adcsEPS[COUNT_ADC_EPS];
extern ADC * adcsPV[COUNT_ADC_PV];

extern DigitalOut statusLED;

extern InterruptIn interruptCtrlSync;
extern InterruptIn interruptBusI2CIRQ;

/******************************** Power Nodes *********************************/
extern CurrentNode * iNodesPV[COUNT_PV];
extern CurrentNode * iNodesPVIn[COUNT_PV];
extern CurrentNode * iNodesPR3V3[COUNT_PR_3V3]; // 13
extern CurrentNode * iNodesPRBatt[COUNT_PR_BATT];
extern CurrentNode * iNodesBatteryHeaters[COUNT_BH];
extern CurrentNode * iNodesDeployables[COUNT_DEPLOY]; //

extern VoltageNode * vNodesPV[COUNT_PV];
extern VoltageNode * vNodesPVIn[COUNT_PV];

extern VoltageNode * vSenseDeployables[COUNT_DEPLOY];

extern DigitalOut * inputSwitching[COUNT_INPUT_SW];

extern CurrentNode iNodeBattInA; //
extern CurrentNode iNodeBattInB; //
extern CurrentNode iNodeBattOutA; //
extern CurrentNode iNodeBattOutB; //
extern CurrentNode iNode3V3In; //
extern CurrentNode iNode3V3Out;
extern CurrentNode iNodePR3V3_EPS; //

extern VoltageNode vNodeBattA;
extern VoltageNode vNodeBattB;
extern VoltageNode vNode3V3;
extern VoltageNode vNodeUmbilical;
extern VoltageNode vSenseEjectTimer;

/******************************** Thermistors *********************************/
extern Thermistor thermistorBattA;
extern Thermistor thermistorBattB;
extern Thermistor thermistorPMIC;
extern Thermistor thermistorRegA;
extern Thermistor thermistorRegB;
extern Thermistor thermistorSwIn0;
extern Thermistor thermistorSwIn1;
extern Thermistor thermistorSwOut0;
extern Thermistor thermistorSwOut1;
extern Thermistor thermistorPCB0;
extern Thermistor thermistorPCB1;
extern Thermistor thermistorPCB2;
extern Thermistor thermistorPCB3;
extern Thermistor thermistorPCB4;

extern Thermistor * thermistorsEPS[COUNT_THERM_BOARD];

extern Thermistor * thermistorsMPPT[COUNT_PV];

/****************************** Internal Sensors ******************************/

extern InternalTemp internalTemp;

#endif /* _SRC_PMIC_OBJECTS_H_ */