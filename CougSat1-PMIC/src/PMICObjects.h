/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PMICObjects.h
 * @author Bradley Davis
 * @date 5 Nov 2018
 * @brief Instantiates all of the PMIC objects
 *
 */

#ifndef _SRC_PMIC_OBJECTS_H_
#define _SRC_PMIC_OBJECTS_H_

#include "components/CDH.h"
#include "components/PowerNode.h"
#include "components/PowerNodeDirect.h"
#include "components/PowerNodeTCA9535.h"
#include "components/Thermistor.h"
#include "drivers/LTC2499.h"
#include "drivers/TCA9535.h"
#include "mbed.h"

/*************************** Interfaces and Drivers ***************************/
extern I2C i2cLocal;
extern I2C i2cBus;

extern CDH cdh;

extern LTC2499 adcEPS0;
extern LTC2499 adcEPS1;
extern LTC2499 adcEPS2;
extern LTC2499 adcEPS3;
extern LTC2499 adcEPS4;
extern LTC2499 adcEPS5;
extern LTC2499 adcPV0;
extern LTC2499 adcPV1;
extern LTC2499 adcPV2;
extern LTC2499 adcPV3;

extern TCA9535 gpioEPS0;
extern TCA9535 gpioEPS1;
extern TCA9535 gpioPV0;
extern TCA9535 gpioPV1;
extern TCA9535 gpioPV2;
extern TCA9535 gpioPV3;

extern DigitalOut led;
extern DigitalOut umbilicalSwitchA;
extern DigitalOut umbilicalSwitchB;

extern InterruptIn interruptCtrlSync;
extern InterruptIn interruptBusI2CIRQ;

/******************************** Power Nodes *********************************/
extern PowerNode nodesPVIn[8];
extern PowerNode nodesPVOut[8];
extern PowerNode nodesPR3V3[13];
extern PowerNode nodesPRBatt[7];
extern PowerNode nodesPV3V3[4];
extern PowerNode nodesBatteryHeaters[2];

extern PowerNodeDirect nodeDeployables;

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