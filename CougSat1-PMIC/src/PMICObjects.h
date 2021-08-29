#ifndef _SRC_PMIC_OBJECTS_H_
#define _SRC_PMIC_OBJECTS_H_

#include "Configuration.h"
#include "interfaces/CDH.h"

#include <ADC/ADC.h>
#include <CurrentNode.h>
#include <Thermistor.h>
#include <mbed.h>

/*************************** Interfaces and Drivers ***************************/
extern I2C      i2cLocal;
extern I2CSlave i2cBus;

extern CDH cdh;

extern ADC * adcEPSs[COUNT_ADC_EPS];
extern ADC * adcPVs[COUNT_ADC_PV];

extern DigitalOut statusLED;

extern InterruptIn interruptCtrlSync;
extern InterruptIn interruptBusI2CIRQ;

/******************************** Power Nodes *********************************/
extern CurrentNode * nodesPVIn[COUNT_PV];
extern CurrentNode * nodesPVOut[COUNT_PV];
extern CurrentNode * nodesPR3V3[COUNT_PR_3V3];
extern CurrentNode * nodesPRBatt[COUNT_PR_BATT];
extern CurrentNode * nodesBatteryHeaters[COUNT_BH];
extern CurrentNode * nodesDeployables[COUNT_DEPLOY];

extern DigitalOut * inputSwitching[COUNT_INPUT_SW];

extern CurrentNode nodeBattInA;
extern CurrentNode nodeBattInB;
extern CurrentNode nodeBattOutA;
extern CurrentNode nodeBattOutB;
extern CurrentNode node3V3InA;
extern CurrentNode node3V3InB;
extern CurrentNode node3V3OutA;
extern CurrentNode node3V3OutB;
extern CurrentNode nodePR3V3_EPS;

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

extern Thermistor thermistorsMPPT[COUNT_PV];
extern Thermistor thermistorsPVBoard[COUNT_PV];

/****************************** Internal Sensors ******************************/

extern AnalogIn internalTemp;
extern AnalogIn internalVref;
extern AnalogIn internalVbat;

#endif /* _SRC_PMIC_OBJECTS_H_ */