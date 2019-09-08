#ifndef _SRC_PMIC_OBJECTS_H_
#define _SRC_PMIC_OBJECTS_H_

#include "Configuration.h"
#include "interfaces/CDH.h"

#include <ADC/ADC.h>
#include <PowerNode.h>
#include <Thermistor.h>
#include <mbed.h>

/*************************** Interfaces and Drivers ***************************/
extern I2C i2cLocal;
extern I2C i2cBus;

extern CDH cdh;

extern ADC * adcEPSs[COUNT_ADC_EPS];
extern ADC * adcPVs[COUNT_ADC_PV];

extern DigitalOut statusLED;

extern InterruptIn interruptCtrlSync;
extern InterruptIn interruptBusI2CIRQ;

/******************************** Power Nodes *********************************/
extern PowerNode * nodesPVIn[COUNT_PV];
extern PowerNode * nodesPVOut[COUNT_PV];
extern PowerNode * nodesPR3V3[COUNT_PR_3V3];
extern PowerNode * nodesPRBatt[COUNT_PR_BATT];
extern PowerNode * nodesPV3V3[COUNT_PV_3V3];
extern PowerNode * nodesBatteryHeaters[COUNT_BH];
extern PowerNode * nodesDeployables[COUNT_DEPLOYABLES];

extern PowerNode nodeVBattA;
extern PowerNode nodeVBattB;
extern PowerNode nodeBattA;
extern PowerNode nodeBattB;
extern PowerNode node3V3InA;
extern PowerNode node3V3InB;
extern PowerNode node3V3OutA;
extern PowerNode node3V3OutB;

/******************************** Thermistors *********************************/
extern Thermistor thermistorBattA;
extern Thermistor thermistorBattB;
extern Thermistor thermistorPMIC;
extern Thermistor thermistorRegA;
extern Thermistor thermistorRegB;
extern Thermistor thermistorpXpY;
extern Thermistor thermistornXpY;
extern Thermistor thermistorpXnY;
extern Thermistor thermistornXnY;

extern Thermistor thermistorsMPPT[8];
extern Thermistor thermistorsPVBoard[12];

#endif /* _SRC_PMIC_OBJECTS_H_ */