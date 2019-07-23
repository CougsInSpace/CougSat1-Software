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

#include "PMICConfiguration.h"
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

extern DigitalOut statusLED;
extern DigitalOut umbilicalSwitchA;
extern DigitalOut umbilicalSwitchB;

extern InterruptIn interruptCtrlSync;
extern InterruptIn interruptBusI2CIRQ;

/******************************** Power Nodes *********************************/
PowerNode nodesPVIn[] = {PowerNode(adcPV0, PIN_ADC_PVIN_I_A, SHUNT_PVIN_0A),
    PowerNode(adcPV0, PIN_ADC_PVIN_I_B, SHUNT_PVIN_0B),
    PowerNode(adcPV1, PIN_ADC_PVIN_I_A, SHUNT_PVIN_1A),
    PowerNode(adcPV1, PIN_ADC_PVIN_I_B, SHUNT_PVIN_1B),
    PowerNode(adcPV2, PIN_ADC_PVIN_I_A, SHUNT_PVIN_2A),
    PowerNode(adcPV2, PIN_ADC_PVIN_I_B, SHUNT_PVIN_2B),
    PowerNode(adcPV3, PIN_ADC_PVIN_I_A, SHUNT_PVIN_3A),
    PowerNode(adcPV3, PIN_ADC_PVIN_I_B, SHUNT_PVIN_3B)
};


PowerNode nodesPVOut[] = {
    PowerNodeTCA9535(adcPV0, PIN_ADC_PVOUT_I_A, SHUNT_PVOUT_0A, gpioEPS0,
        PIN_PV_SW_0A, PIN_PV_SW_0B),
    PowerNodeTCA9535(adcPV0, PIN_ADC_PVOUT_I_B, SHUNT_PVOUT_0B, gpioEPS0,
        PIN_PV_SW_1A, PIN_PV_SW_1B),
    PowerNodeTCA9535(adcPV1, PIN_ADC_PVOUT_I_A, SHUNT_PVOUT_1A, gpioEPS0,
        PIN_PV_SW_2A, PIN_PV_SW_2B),
    PowerNodeTCA9535(adcPV1, PIN_ADC_PVOUT_I_B, SHUNT_PVOUT_1B, gpioEPS0,
        PIN_PV_SW_3A, PIN_PV_SW_3B),
    PowerNodeTCA9535(adcPV2, PIN_ADC_PVOUT_I_A, SHUNT_PVOUT_2A, gpioEPS1,
        PIN_PV_SW_4A, PIN_PV_SW_4B),
    PowerNodeTCA9535(adcPV2, PIN_ADC_PVOUT_I_B, SHUNT_PVOUT_2B, gpioEPS1,
        PIN_PV_SW_5A, PIN_PV_SW_5B),
    PowerNodeTCA9535(adcPV3, PIN_ADC_PVOUT_I_A, SHUNT_PVOUT_3A, gpioEPS1,
        PIN_PV_SW_6A, PIN_PV_SW_6B),
    PowerNodeTCA9535(adcPV3, PIN_ADC_PVOUT_I_B, SHUNT_PVOUT_3B, gpioEPS1,
        PIN_PV_SW_7A, PIN_PV_SW_7B)
};


PowerNode nodesPR3V3[] =
 {
    PowerNodeTCA9535(adcEPS0, PIN_ADC_PR_3V3_0, SHUNT_PR_3V3_0, gpioEPS1, PIN_PC_3V3_0A, PIN_PC_3V3_0B, priority_PR3V3_four,PR3V3_four),   // IHU
    PowerNodeTCA9535(adcEPS0, PIN_ADC_PR_3V3_1, SHUNT_PR_3V3_1, gpioEPS1, PIN_PC_3V3_1A, PIN_PC_3V3_1B, priority_PR3V3_five,PR3V3_five ),  // IHU : SD Card
    PowerNodeDirect(adcEPS0, PIN_ADC_PR_3V3_2, SHUNT_PR_3V3_2, PIN_PC_3V3_2A, PIN_PC_3V3_2B, priority_PR3V3_six, PR3V3_six),               // ADCS 
    PowerNodeDirect(adcEPS0, PIN_ADC_PR_3V3_3, SHUNT_PR_3V3_3, PIN_PC_3V3_3A, PIN_PC_3V3_3B, priority_PR3V3_seven, PR3V3_seven),           // ADCS: IMUs
    PowerNodeDirect(adcEPS0, PIN_ADC_PR_3V3_4, SHUNT_PR_3V3_4, PIN_PC_3V3_4A, PIN_PC_3V3_4B, priority_PR3V3_nine, PR3V3_nine),             // ADCS: GPS
    PowerNodeDirect(adcEPS0, PIN_ADC_PR_3V3_5, SHUNT_PR_3V3_5, PIN_PC_3V3_5A, PIN_PC_3V3_5B, priority_PR3V3_one, PR3V3_one),               // IFJR & SD Card
    PowerNodeDirect(adcEPS0, PIN_ADC_PR_3V3_6, SHUNT_PR_3V3_6, PIN_PC_3V3_6A, PIN_PC_3V3_6B, priority_PR3V3_two, PR3V3_two),               // Communications & Radios
    PowerNodeDirect(adcEPS0, PIN_ADC_PR_3V3_7, SHUNT_PR_3V3_7, PIN_PC_3V3_7A, PIN_PC_3V3_7B, priority_PR3V3_eleven, PR3V3_eleven),         // Germination Lighting
    PowerNodeDirect(adcEPS2, PIN_ADC_PR_3V3_8, SHUNT_PR_3V3_8, PIN_PC_3V3_8A, PIN_PC_3V3_8B, priority_PR3V3_fifteen, PR3V3_fifteen),       // Germination Camera
    PowerNodeDirect(adcEPS2, PIN_ADC_PR_3V3_9, SHUNT_PR_3V3_9, PIN_PC_3V3_9A,  PIN_PC_3V3_9B, priority_PR3V3_twelve, PR3V3_twelve),        // Germinatio Sensors
    PowerNodeDirect(adcEPS2, PIN_ADC_PR_3V3_10, SHUNT_PR_3V3_10, PIN_PC_3V3_10A, PIN_PC_3V3_10B, priority_PR3V3_thirteen, PR3V3_thirteen), // Ionizing Radiation Sensor
    PowerNodeDirect(adcEPS2, PIN_ADC_PR_3V3_11, SHUNT_PR_3V3_11, PIN_PC_3V3_11A,  PIN_PC_3V3_11B, priority_PR3V3_fourteen, PR3V3_fourteen),// Ground Camera
    PowerNodeDirect(adcEPS1, PIN_ADC_PR_3V3_12, SHUNT_PR_3V3_12, PIN_PC_3V3_12A,  PIN_PC_3V3_12B, UNUSED, UNUSED)                          // Unused
};

PowerNode nodesPRBatt[] = 
{
	PowerNodeDirect(adcEPS1, PIN_ADC_PR_BATT_0, SHUNT_PR_BATT_0, PIN_PC_BATT_0A, PIN_PC_BATT_0B, priority_PRBATT_ten, PRBATT_ten),      // Germination Heater
    PowerNodeDirect(adcEPS1, PIN_ADC_PR_BATT_1, SHUNT_PR_BATT_1, PIN_PC_BATT_1A, PIN_PC_BATT_1B, UNUSED, UNUSED),                       // Unused
    PowerNodeDirect(adcEPS1, PIN_ADC_PR_BATT_2, SHUNT_PR_BATT_2, PIN_PC_BATT_2A, PIN_PC_BATT_2B, UNUSED, UNUSED),                       // Unused
    PowerNodeDirect(adcEPS1, PIN_ADC_PR_BATT_3, SHUNT_PR_BATT_3, PIN_PC_BATT_3A, PIN_PC_BATT_3B, UNUSED, UNUSED),                       // Unused
    PowerNodeDirect(adcEPS1, PIN_ADC_PR_BATT_4, SHUNT_PR_BATT_4, PIN_PC_BATT_4A, PIN_PC_BATT_4B, UNUSED, UNUSED),                       // Unused
    PowerNodeDirect(adcEPS1, PIN_ADC_PR_BATT_5, SHUNT_PR_BATT_5, PIN_PC_BATT_5A, PIN_PC_BATT_5B, priority_PRBATT_eight, PRBATT_eight), // ADCS: Magnetorquers
    PowerNodeDirect(adcEPS1, PIN_ADC_PR_BATT_6, SHUNT_PR_BATT_6, PIN_PC_BATT_6A, PIN_PC_BATT_6B, priority_PRBATT_three, PRBATT_three)   // Comms: TX Amplifier
};

PowerNode nodesPV3V3[] = {
    PowerNodeTCA9535(adcEPS4, PIN_ADC_PV_3V3_0, SHUNT_PV_3V3_0, gpioEPS0, PIN_PVC_3V3_0A, PIN_PVC_3V3_0B), 
    PowerNodeTCA9535(adcEPS4, PIN_ADC_PV_3V3_1, SHUNT_PV_3V3_1, gpioEPS0, PIN_PVC_3V3_1A, PIN_PVC_3V3_1B), 
    PowerNodeTCA9535(adcEPS2, PIN_ADC_PV_3V3_2, SHUNT_PV_3V3_2, gpioEPS1, PIN_PVC_3V3_2A, PIN_PVC_3V3_2B),
    PowerNodeTCA9535(adcEPS2, PIN_ADC_PV_3V3_3, SHUNT_PV_3V3_3, gpioEPS1, PIN_PVC_3V3_3A, PIN_PVC_3V3_3B)
};

PowerNode nodesBatteryHeaters[] = {
    PowerNodeTCA9535(adcEPS4, PIN_ADC_PR_BH_0, SHUNT_BH_0, gpioEPS0,
        PIN_PC_BH_0A, PIN_PC_BH_0B),
    PowerNodeTCA9535(adcEPS3, PIN_ADC_PR_BH_1, SHUNT_BH_1, gpioEPS0,
        PIN_PC_BH_1A, PIN_PC_BH_1B)};

PowerNodeDirect nodeDeployables(adcEPS3, PIN_ADC_PR_DEPLOY, SHUNT_DEPLOYABLES, PIN_PC_DEPLOY_A, PIN_PC_DEPLOY_B);

PowerNode nodeVBattA(adcEPS4, PIN_ADC_VBATT_I_A, SHUNT_VBATT_A);
PowerNode nodeVBattB(adcEPS3, PIN_ADC_VBATT_I_B, SHUNT_VBATT_B);
PowerNode nodeBattA(adcEPS4, PIN_ADC_BATT_A, SHUNT_BATT_A);
PowerNode nodeBattB(adcEPS4, PIN_ADC_BATT_B, SHUNT_BATT_B);
PowerNode node3V3InA(adcEPS4, PIN_ADC_3V3_IN_A, SHUNT_3V3_IN_A);
PowerNode node3V3InB(adcEPS3, PIN_ADC_3V3_IN_B, SHUNT_3V3_IN_B);
PowerNode node3V3OutA(adcEPS4, PIN_ADC_3V3_OUT_A, SHUNT_3V3_OUT_A);
PowerNode node3V3OutB(adcEPS3, PIN_ADC_3V3_OUT_B, SHUNT_3V3_OUT_B);

// extern PowerNode * nodesPVIn[COUNT_PV];
// extern PowerNode * nodesPVOut[COUNT_PV];
// extern PowerNode * nodesPR3V3[COUNT_PR_3V3];
// extern PowerNode * nodesPRBatt[COUNT_PR_BATT];
// extern PowerNode * nodesPV3V3[COUNT_PV_3V3];
// extern PowerNode * nodesBatteryHeaters[COUNT_BH];

// extern PowerNodeDirect nodeDeployables;

// extern PowerNode nodeVBattA;
// extern PowerNode nodeVBattB;
// extern PowerNode nodeBattA;
// extern PowerNode nodeBattB;
// extern PowerNode node3V3InA;
// extern PowerNode node3V3InB;
// extern PowerNode node3V3OutA;
// extern PowerNode node3V3OutB;

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