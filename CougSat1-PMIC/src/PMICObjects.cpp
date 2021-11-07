#include "PMICObjects.h"

#include <ADC/AD7291.h>
#include <ADC/AD7689.h>

/*************************** Interfaces and Drivers ***************************/
I2C      i2cLocal(I2C_SDA, I2C_SCL);
I2CSlave i2cBus(BUS_I2C_SDA, BUS_I2C_SCL);

CDH cdh(i2cBus);

AD7689 adcEPS0(SPI_MOSI, SPI_MISO, SPI_SCK, ADC_SEL_0_N, ADC_AVREF_0);
AD7689 adcEPS1(SPI_MOSI, SPI_MISO, SPI_SCK, ADC_SEL_1_N, ADC_AVREF_1);
AD7689 adcEPS2(SPI_MOSI, SPI_MISO, SPI_SCK, ADC_SEL_2_N, ADC_AVREF_2);

AD7291 adcEPS3(i2cLocal, AD7291Addr_t::LH, ADC_AVREF_3);
AD7291 adcEPS4(i2cLocal, AD7291Addr_t::LF, ADC_AVREF_4);
AD7291 adcEPS5(i2cLocal, AD7291Addr_t::LL, ADC_AVREF_5);
AD7291 adcEPS6(i2cLocal, AD7291Addr_t::FL, ADC_AVREF_6);
AD7291 adcEPS7(i2cLocal, AD7291Addr_t::HH, ADC_AVREF_7);

AD7291 adcPV0(i2cLocal, AD7291Addr_t::FF, ADC_AVREF_PV0);
AD7291 adcPV1(i2cLocal, AD7291Addr_t::FH, ADC_AVREF_PV1);
AD7291 adcPV2(i2cLocal, AD7291Addr_t::HF, ADC_AVREF_PV2);
AD7291 adcPV3(i2cLocal, AD7291Addr_t::HL, ADC_AVREF_PV3);

// clang-format off
ADC * adcsEPS[COUNT_ADC_EPS] = {
  &adcEPS0,
  &adcEPS1,
  &adcEPS2,
  &adcEPS3,
  &adcEPS4,
  &adcEPS5,
  &adcEPS6,
  &adcEPS7};
ADC * adcsPV[COUNT_ADC_PV]   = {&adcPV0, &adcPV1, &adcPV2, &adcPV3};
// clang-format on

DigitalOut statusLED(LED1);

InterruptIn interruptCtrlSync(CTRL_SYNC);
InterruptIn interruptBusI2CIRQ(BUS_I2C_IRQ);

/******************************** Power Nodes *********************************/
CurrentNode iNodePV_0A(adcPV0, ADCChannel_t::CM_00, GAIN_PV_0A);
CurrentNode iNodePV_0B(adcPV0, ADCChannel_t::CM_06, GAIN_PV_0B);
CurrentNode iNodePV_1A(adcPV1, ADCChannel_t::CM_00, GAIN_PV_1A);
CurrentNode iNodePV_1B(adcPV1, ADCChannel_t::CM_06, GAIN_PV_1B);
CurrentNode iNodePV_2A(adcPV2, ADCChannel_t::CM_00, GAIN_PV_2A);
CurrentNode iNodePV_2B(adcPV2, ADCChannel_t::CM_06, GAIN_PV_2B);
CurrentNode iNodePV_3A(adcPV3, ADCChannel_t::CM_00, GAIN_PV_3A);
CurrentNode iNodePV_3B(adcPV3, ADCChannel_t::CM_06, GAIN_PV_3B);

CurrentNode * iNodesPV[COUNT_PV] = {&iNodePV_0A, &iNodePV_0B, &iNodePV_1A,
    &iNodePV_1B, &iNodePV_2A, &iNodePV_2B, &iNodePV_3A, &iNodePV_3B};

VoltageNode vNodePV_0A(adcPV0, ADCChannel_t::CM_04, GAIN_V_PV_0A);
VoltageNode vNodePV_0B(adcPV0, ADCChannel_t::CM_05, GAIN_V_PV_0B);
VoltageNode vNodePV_1A(adcPV1, ADCChannel_t::CM_04, GAIN_V_PV_1A);
VoltageNode vNodePV_1B(adcPV1, ADCChannel_t::CM_05, GAIN_V_PV_1B);
VoltageNode vNodePV_2A(adcPV2, ADCChannel_t::CM_04, GAIN_V_PV_2A);
VoltageNode vNodePV_2B(adcPV2, ADCChannel_t::CM_05, GAIN_V_PV_2B);
VoltageNode vNodePV_3A(adcPV3, ADCChannel_t::CM_04, GAIN_V_PV_3A);
VoltageNode vNodePV_3B(adcPV3, ADCChannel_t::CM_05, GAIN_V_PV_3B);

VoltageNode * vNodesPV[COUNT_PV] = {&vNodePV_0A, &vNodePV_0B, &vNodePV_1A,
    &vNodePV_1B, &vNodePV_2A, &vNodePV_2B, &vNodePV_3A, &vNodePV_3B};

CurrentNode iNodePVIn_0A(adcPV0, ADCChannel_t::CM_01, GAIN_PVIN_0A);
CurrentNode iNodePVIn_0B(adcPV0, ADCChannel_t::CM_07, GAIN_PVIN_0B);
CurrentNode iNodePVIn_1A(adcPV1, ADCChannel_t::CM_01, GAIN_PVIN_1A);
CurrentNode iNodePVIn_1B(adcPV1, ADCChannel_t::CM_07, GAIN_PVIN_1B);
CurrentNode iNodePVIn_2A(adcPV2, ADCChannel_t::CM_01, GAIN_PVIN_2A);
CurrentNode iNodePVIn_2B(adcPV2, ADCChannel_t::CM_07, GAIN_PVIN_2B);
CurrentNode iNodePVIn_3A(adcPV3, ADCChannel_t::CM_01, GAIN_PVIN_3A);
CurrentNode iNodePVIn_3B(adcPV3, ADCChannel_t::CM_07, GAIN_PVIN_3B);

CurrentNode * iNodesPVIn[COUNT_PV] = {&iNodePVIn_0A, &iNodePVIn_0B,
    &iNodePVIn_1A, &iNodePVIn_1B, &iNodePVIn_2A, &iNodePVIn_2B, &iNodePVIn_3A,
    &iNodePVIn_3B};

VoltageNode vNodePVIn_0A(adcEPS1, ADCChannel_t::CM_00, GAIN_V_PVIN_0A);
VoltageNode vNodePVIn_0B(adcEPS1, ADCChannel_t::CM_01, GAIN_V_PVIN_0B);
VoltageNode vNodePVIn_1A(adcEPS1, ADCChannel_t::CM_02, GAIN_V_PVIN_1A);
VoltageNode vNodePVIn_1B(adcEPS1, ADCChannel_t::CM_03, GAIN_V_PVIN_1B);
VoltageNode vNodePVIn_2A(adcEPS0, ADCChannel_t::CM_01, GAIN_V_PVIN_2A);
VoltageNode vNodePVIn_2B(adcEPS0, ADCChannel_t::CM_03, GAIN_V_PVIN_2B);
VoltageNode vNodePVIn_3A(adcEPS0, ADCChannel_t::CM_00, GAIN_V_PVIN_3A);
VoltageNode vNodePVIn_3B(adcEPS0, ADCChannel_t::CM_02, GAIN_V_PVIN_3B);

VoltageNode * vNodesPVIn[COUNT_PV] = {&vNodePVIn_0A, &vNodePVIn_0B,
    &vNodePVIn_1A, &vNodePVIn_1B, &vNodePVIn_2A, &vNodePVIn_2B, &vNodePVIn_3A,
    &vNodePVIn_3B};

// DigitalOut inputSwitch_02A(PC_PV02A, true); // PC_PV02A was removed on new
// version of PMIC DigitalOut inputSwitch_02B(PC_PV02B, true); DigitalOut
// inputSwitch_13A(PC_PV13A, true); DigitalOut inputSwitch_13B(PC_PV13B, true);

// DigitalOut * inputSwitching[COUNT_INPUT_SW] = {
//     &inputSwitch_02A, &inputSwitch_02B, &inputSwitch_13A, &inputSwitch_13B};

// clang-format off
CurrentNode iNodePR3V3_0(adcEPS3, ADCChannel_t::CM_07, GAIN_PR_3V3_0, PC_3V3_0, false, INITIAL_PR_3V3_0, RANK_PR_3V3_0); // done
CurrentNode iNodePR3V3_1(adcEPS3, ADCChannel_t::CM_06, GAIN_PR_3V3_1, PC_3V3_1, false, INITIAL_PR_3V3_1, RANK_PR_3V3_1); // done
CurrentNode iNodePR3V3_2(adcEPS3, ADCChannel_t::CM_05, GAIN_PR_3V3_2, PC_3V3_2, false, INITIAL_PR_3V3_2, RANK_PR_3V3_2); // done
CurrentNode iNodePR3V3_3(adcEPS3, ADCChannel_t::CM_04, GAIN_PR_3V3_3, PC_3V3_3, false, INITIAL_PR_3V3_3, RANK_PR_3V3_3);
CurrentNode iNodePR3V3_4(adcEPS3, ADCChannel_t::CM_03, GAIN_PR_3V3_4, PC_3V3_4, false, INITIAL_PR_3V3_4, RANK_PR_3V3_4);
CurrentNode iNodePR3V3_5(adcEPS3, ADCChannel_t::CM_02, GAIN_PR_3V3_5, PC_3V3_5, false, INITIAL_PR_3V3_5, RANK_PR_3V3_5);
CurrentNode iNodePR3V3_6(adcEPS3, ADCChannel_t::CM_01, GAIN_PR_3V3_6, PC_3V3_6, false, INITIAL_PR_3V3_6, RANK_PR_3V3_6);
CurrentNode iNodePR3V3_7(adcEPS3, ADCChannel_t::CM_00, GAIN_PR_3V3_7, PC_3V3_7, false, INITIAL_PR_3V3_7, RANK_PR_3V3_7);
CurrentNode iNodePR3V3_8(adcEPS4, ADCChannel_t::CM_07, GAIN_PR_3V3_8, PC_3V3_8, false, INITIAL_PR_3V3_8, RANK_PR_3V3_8);
CurrentNode iNodePR3V3_9(adcEPS4, ADCChannel_t::CM_06, GAIN_PR_3V3_9, PC_3V3_9, false, INITIAL_PR_3V3_9, RANK_PR_3V3_9);
CurrentNode iNodePR3V3_10(adcEPS4, ADCChannel_t::CM_05, GAIN_PR_3V3_10, PC_3V3_10, false, INITIAL_PR_3V3_10, RANK_PR_3V3_10);
CurrentNode iNodePR3V3_11(adcEPS4, ADCChannel_t::CM_04, GAIN_PR_3V3_11, PC_3V3_11, false, INITIAL_PR_3V3_11, RANK_PR_3V3_11);
CurrentNode iNodePR3V3_12(adcEPS4, ADCChannel_t::CM_03, GAIN_PR_3V3_12, PC_3V3_12, false, INITIAL_PR_3V3_12, RANK_PR_3V3_12);
CurrentNode iNodePR3V3_PV(adcEPS5, ADCChannel_t::CM_07, GAIN_PR_3V3_PV, PC_3V3_PV, false, INITIAL_PR_3V3_PV, RANK_PR_3V3_PV);

CurrentNode * iNodesPR3V3[COUNT_PR_3V3] = {
  &iNodePR3V3_0,
  &iNodePR3V3_1,
  &iNodePR3V3_2,
  &iNodePR3V3_3,
  &iNodePR3V3_4,
  &iNodePR3V3_5,
  &iNodePR3V3_6,
  &iNodePR3V3_7,
  &iNodePR3V3_8,
  &iNodePR3V3_9,
  &iNodePR3V3_10,
  &iNodePR3V3_11,
  &iNodePR3V3_12,
  &iNodePR3V3_PV};

CurrentNode iNodePRBatt_0(adcEPS4, ADCChannel_t::CM_02, GAIN_PR_BATT_0, PC_BATT_0, false, INITIAL_PR_BATT_0, RANK_PR_BATT_0);
CurrentNode iNodePRBatt_1(adcEPS4, ADCChannel_t::CM_01, GAIN_PR_BATT_1, PC_BATT_1, false, INITIAL_PR_BATT_1, RANK_PR_BATT_1);
CurrentNode iNodePRBatt_2(adcEPS4, ADCChannel_t::CM_00, GAIN_PR_BATT_2, PC_BATT_2, false, INITIAL_PR_BATT_2, RANK_PR_BATT_2);
CurrentNode iNodePRBatt_3(adcEPS5, ADCChannel_t::CM_03, GAIN_PR_BATT_3, PC_BATT_3, false, INITIAL_PR_BATT_3, RANK_PR_BATT_3);
CurrentNode iNodePRBatt_4(adcEPS5, ADCChannel_t::CM_02, GAIN_PR_BATT_4, PC_BATT_4, false, INITIAL_PR_BATT_4, RANK_PR_BATT_4);
CurrentNode iNodePRBatt_5(adcEPS5, ADCChannel_t::CM_01, GAIN_PR_BATT_5, PC_BATT_5, false, INITIAL_PR_BATT_5, RANK_PR_BATT_5);
CurrentNode iNodePRBatt_6(adcEPS5, ADCChannel_t::CM_00, GAIN_PR_BATT_6, PC_BATT_6, false, INITIAL_PR_BATT_6, RANK_PR_BATT_6);

CurrentNode * iNodesPRBatt[COUNT_PR_BATT] = {
  &iNodePRBatt_0,
  &iNodePRBatt_1,
  &iNodePRBatt_2,
  &iNodePRBatt_3,
  &iNodePRBatt_4,
  &iNodePRBatt_5,
  &iNodePRBatt_6};
  
CurrentNode iNodeBH_0(adcEPS5, ADCChannel_t::CM_04, GAIN_BH_0, PC_BH_0, false, INITIAL_BH_0);
CurrentNode iNodeBH_1(adcEPS5, ADCChannel_t::CM_05, GAIN_BH_1, PC_BH_1, false, INITIAL_BH_1);

CurrentNode * iNodesBatteryHeaters[COUNT_BH] = {&iNodeBH_0, &iNodeBH_1};

CurrentNode iNodeDeploy_0(adcEPS6, ADCChannel_t::CM_07, GAIN_DEPLOY_0, PC_DEPLOY_0, false, INITIAL_DEPLOY_0);
CurrentNode iNodeDeploy_1(adcEPS7, ADCChannel_t::CM_00, GAIN_DEPLOY_1, PC_DEPLOY_1, false, INITIAL_DEPLOY_1);

CurrentNode * iNodesDeployables[COUNT_DEPLOY] = {&iNodeDeploy_0, &iNodeDeploy_1};

VoltageNode vSenseDeploy_0(adcEPS2, ADCChannel_t::CM_02, GAIN_V_DEPLOY_0);
VoltageNode vSenseDeploy_1(adcEPS0, ADCChannel_t::CM_07, GAIN_V_DEPLOY_1);

VoltageNode * vSenseDeployables[COUNT_DEPLOY] = {&vSenseDeploy_0, &vSenseDeploy_1};
// clang-format on

CurrentNode iNodeBattInA(adcEPS2, ADCChannel_t::CM_05, GAIN_BATT_IN_A);
CurrentNode iNodeBattInB(adcEPS2, ADCChannel_t::CM_03, GAIN_BATT_IN_B);
CurrentNode iNodeBattOutA(adcEPS1, ADCChannel_t::CM_04, GAIN_BATT_OUT_A);
CurrentNode iNodeBattOutB(adcEPS2, ADCChannel_t::CM_04, GAIN_BATT_OUT_B);
VoltageNode vNodeBattA(adcEPS2, ADCChannel_t::CM_00, GAIN_V_BATT_A);
VoltageNode vNodeBattB(adcEPS2, ADCChannel_t::CM_07, GAIN_V_BATT_B);

CurrentNode iNode3V3InA(adcEPS1, ADCChannel_t::CM_05, GAIN_3V3_IN_A);
CurrentNode iNode3V3InB(adcEPS0, ADCChannel_t::CM_06, GAIN_3V3_IN_B);
CurrentNode iNode3V3OutA(adcEPS1, ADCChannel_t::CM_06, GAIN_3V3_OUT_A);
CurrentNode iNode3V3OutB(adcEPS0, ADCChannel_t::CM_05, GAIN_3V3_OUT_B);
VoltageNode vNode3V3A(adcEPS1, ADCChannel_t::CM_07, GAIN_V_3V3_A);
VoltageNode vNode3V3B(adcEPS0, ADCChannel_t::CM_04, GAIN_V_3V3_B);

CurrentNode iNodePR3V3_EPS(adcEPS5, ADCChannel_t::CM_06, GAIN_PR_3V3_EPS);

VoltageNode vNodeUmbilical(adcEPS2, ADCChannel_t::CM_06, GAIN_V_UMB);
VoltageNode vSenseEjectTimer(adcEPS2, ADCChannel_t::CM_01, GAIN_V_EJECT);

/******************************** Thermistors *********************************/
// clang-format off
Thermistor thermistorBattA(adcEPS6, ADCChannel_t::CM_05, THERM_BATT_A_300K, THERM_BATT_A_340K);
Thermistor thermistorBattB(adcEPS7, ADCChannel_t::CM_04, THERM_BATT_B_300K, THERM_BATT_B_340K);
Thermistor thermistorPMIC(adcEPS7, ADCChannel_t::CM_05, THERM_PMIC_300K, THERM_PMIC_340K);
Thermistor thermistorRegA(adcEPS6, ADCChannel_t::CM_02, THERM_REG_A_300K, THERM_REG_A_340K);
Thermistor thermistorRegB(adcEPS7, ADCChannel_t::CM_07, THERM_REG_A_300K, THERM_REG_A_340K);
Thermistor thermistorSwIn0(adcEPS6, ADCChannel_t::CM_03, THERM_SW_IN_0_300K, THERM_SW_IN_0_340K);
Thermistor thermistorSwIn1(adcEPS7, ADCChannel_t::CM_06, THERM_SW_IN_1_300K, THERM_SW_IN_1_340K);
Thermistor thermistorSwOut0(adcEPS6, ADCChannel_t::CM_00, THERM_SW_OUT_0_300K, THERM_SW_OUT_0_340K);
Thermistor thermistorSwOut1(adcEPS6, ADCChannel_t::CM_01, THERM_SW_OUT_1_300K, THERM_SW_OUT_1_340K);
Thermistor thermistorPCB0(adcEPS7, ADCChannel_t::CM_01, THERM_PCB_0_300K, THERM_PCB_0_340K);
Thermistor thermistorPCB1(adcEPS6, ADCChannel_t::CM_06, THERM_PCB_1_300K, THERM_PCB_1_340K);
Thermistor thermistorPCB2(adcEPS7, ADCChannel_t::CM_02, THERM_PCB_2_300K, THERM_PCB_2_340K);
Thermistor thermistorPCB3(adcEPS6, ADCChannel_t::CM_04, THERM_PCB_3_300K, THERM_PCB_3_340K);
Thermistor thermistorPCB4(adcEPS7, ADCChannel_t::CM_03, THERM_PCB_4_300K, THERM_PCB_4_340K);

Thermistor * thermistorsEPS[COUNT_THERM_BOARD] = {
  &thermistorBattA,
  &thermistorBattB,
  &thermistorPMIC,
  &thermistorRegA,
  &thermistorRegB,
  &thermistorSwIn0,
  &thermistorSwIn1,
  &thermistorSwOut0,
  &thermistorSwOut1,
  &thermistorPCB0,
  &thermistorPCB1,
  &thermistorPCB2,
  &thermistorPCB3,
  &thermistorPCB4};

Thermistor thermistorMPPT0A(adcPV0, ADCChannel_t::CM_02, THERM_PV0_MPPT_A_300K, THERM_PV0_MPPT_A_340K);
Thermistor thermistorMPPT0B(adcPV0, ADCChannel_t::CM_03, THERM_PV0_MPPT_B_300K, THERM_PV0_MPPT_B_340K);
Thermistor thermistorMPPT1A(adcPV1, ADCChannel_t::CM_02, THERM_PV1_MPPT_A_300K, THERM_PV1_MPPT_A_340K);
Thermistor thermistorMPPT1B(adcPV1, ADCChannel_t::CM_03, THERM_PV1_MPPT_B_300K, THERM_PV1_MPPT_B_340K);
Thermistor thermistorMPPT2A(adcPV2, ADCChannel_t::CM_02, THERM_PV2_MPPT_A_300K, THERM_PV2_MPPT_A_340K);
Thermistor thermistorMPPT2B(adcPV2, ADCChannel_t::CM_03, THERM_PV2_MPPT_B_300K, THERM_PV2_MPPT_B_340K);
Thermistor thermistorMPPT3A(adcPV3, ADCChannel_t::CM_02, THERM_PV3_MPPT_A_300K, THERM_PV3_MPPT_A_340K);
Thermistor thermistorMPPT3B(adcPV3, ADCChannel_t::CM_03, THERM_PV3_MPPT_B_300K, THERM_PV3_MPPT_B_340K);

Thermistor * thermistorsMPPT[COUNT_PV] = {
    &thermistorMPPT0A,
    &thermistorMPPT0B,
    &thermistorMPPT1A,
    &thermistorMPPT1B,
    &thermistorMPPT2A,
    &thermistorMPPT2B,
    &thermistorMPPT3A,
    &thermistorMPPT3B};
// clang-format on

/****************************** Internal Sensors ******************************/
InternalTemp internalTemp;
