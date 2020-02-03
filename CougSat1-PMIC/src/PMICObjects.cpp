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
ADC * adcEPSs[COUNT_ADC_EPS] = {
  &adcEPS0,
  &adcEPS1,
  &adcEPS2,
  &adcEPS3,
  &adcEPS4,
  &adcEPS5,
  &adcEPS6,
  &adcEPS7};
ADC * adcPVs[COUNT_ADC_PV]   = {&adcPV0, &adcPV1, &adcPV2, &adcPV3};
// clang-format on

DigitalOut statusLED(LED1);

InterruptIn interruptCtrlSync(CTRL_SYNC);
InterruptIn interruptBusI2CIRQ(BUS_I2C_IRQ);

/******************************** Power Nodes *********************************/
// CurrentNode   nodePVIn_0A(adcPV0, ADCChannel_t, GAIN_PVIN_0A);
// CurrentNode   nodePVIn_0B(adcPV0, PIN_ADC_PVIN_I_B, GAIN_PVIN_0B);
// CurrentNode   nodePVIn_1A(adcPV1, PIN_ADC_PVIN_I_A, GAIN_PVIN_1A);
// CurrentNode   nodePVIn_1B(adcPV1, PIN_ADC_PVIN_I_B, GAIN_PVIN_1B);
// CurrentNode   nodePVIn_2A(adcPV2, PIN_ADC_PVIN_I_A, GAIN_PVIN_2A);
// CurrentNode   nodePVIn_2B(adcPV2, PIN_ADC_PVIN_I_B, GAIN_PVIN_2B);
// CurrentNode   nodePVIn_3A(adcPV3, PIN_ADC_PVIN_I_A, GAIN_PVIN_3A);
// CurrentNode   nodePVIn_3B(adcPV3, PIN_ADC_PVIN_I_B, GAIN_PVIN_3B);
// CurrentNode * nodesPVIn[COUNT_PV] = {&nodePVIn_0A, &nodePVIn_0B,
// &nodePVIn_1A,
//     &nodePVIn_1B, &nodePVIn_2A, &nodePVIn_2B, &nodePVIn_3A, &nodePVIn_3B};

// CurrentNodeTCA9535 nodePVOut_0A(adcPV0, PIN_ADC_PVOUT_I_A, GAIN_PVOUT_0A,
//     gpioEPS0, PIN_PV_SW_0A, PIN_PV_SW_0B);
// CurrentNodeTCA9535 nodePVOut_0B(adcPV0, PIN_ADC_PVOUT_I_A, GAIN_PVOUT_0B,
//     gpioEPS0, PIN_PV_SW_1A, PIN_PV_SW_1B);
// CurrentNodeTCA9535 nodePVOut_1A(adcPV0, PIN_ADC_PVOUT_I_A, GAIN_PVOUT_1A,
//     gpioEPS0, PIN_PV_SW_2A, PIN_PV_SW_2B);
// CurrentNodeTCA9535 nodePVOut_1B(adcPV0, PIN_ADC_PVOUT_I_A, GAIN_PVOUT_1B,
//     gpioEPS0, PIN_PV_SW_3A, PIN_PV_SW_3B);
// CurrentNodeTCA9535 nodePVOut_2A(adcPV0, PIN_ADC_PVOUT_I_A, GAIN_PVOUT_2A,
//     gpioEPS1, PIN_PV_SW_4A, PIN_PV_SW_4B);
// CurrentNodeTCA9535 nodePVOut_2B(adcPV0, PIN_ADC_PVOUT_I_A, GAIN_PVOUT_2B,
//     gpioEPS1, PIN_PV_SW_5A, PIN_PV_SW_5B);
// CurrentNodeTCA9535 nodePVOut_3A(adcPV0, PIN_ADC_PVOUT_I_A, GAIN_PVOUT_3A,
//     gpioEPS1, PIN_PV_SW_6A, PIN_PV_SW_6B);
// CurrentNodeTCA9535 nodePVOut_3B(adcPV0, PIN_ADC_PVOUT_I_A, GAIN_PVOUT_3B,
//     gpioEPS1, PIN_PV_SW_7A, PIN_PV_SW_7B);

// CurrentNode * nodesPVOut[COUNT_PV] = {&nodePVOut_0A, &nodePVOut_0B,
// &nodePVOut_1A,
//     &nodePVOut_1B, &nodePVOut_2A, &nodePVOut_2B, &nodePVOut_3A,
//     &nodePVOut_3B};

DigitalOut inputSwitch_02A(PC_PV02A, 1);
DigitalOut inputSwitch_02B(PC_PV02B, 1);
DigitalOut inputSwitch_13A(PC_PV13A, 1);
DigitalOut inputSwitch_13B(PC_PV13B, 1);

DigitalOut * inputSwitching[COUNT_INPUT_SW] = {
    &inputSwitch_02A, &inputSwitch_02B, &inputSwitch_13A, &inputSwitch_13B};

// clang-format off
CurrentNode nodePR3V3_0(adcEPS3, ADCChannel_t::CM_07, GAIN_PR_3V3_0, PC_3V3_0_N, true, RANK_PR_3V3_0);
CurrentNode nodePR3V3_1(adcEPS3, ADCChannel_t::CM_06, GAIN_PR_3V3_1, PC_3V3_1_N, true, RANK_PR_3V3_1);
CurrentNode nodePR3V3_2(adcEPS3, ADCChannel_t::CM_05, GAIN_PR_3V3_2, PC_3V3_2_N, true, RANK_PR_3V3_2);
CurrentNode nodePR3V3_3(adcEPS3, ADCChannel_t::CM_04, GAIN_PR_3V3_3, PC_3V3_3_N, true, RANK_PR_3V3_3);
CurrentNode nodePR3V3_4(adcEPS3, ADCChannel_t::CM_03, GAIN_PR_3V3_4, PC_3V3_4_N, true, RANK_PR_3V3_4);
CurrentNode nodePR3V3_5(adcEPS3, ADCChannel_t::CM_02, GAIN_PR_3V3_5, PC_3V3_5, false, RANK_PR_3V3_5);
CurrentNode nodePR3V3_6(adcEPS3, ADCChannel_t::CM_01, GAIN_PR_3V3_6, PC_3V3_6_N, true, RANK_PR_3V3_6);
CurrentNode nodePR3V3_7(adcEPS3, ADCChannel_t::CM_00, GAIN_PR_3V3_7, PC_3V3_7_N, true, RANK_PR_3V3_7);
CurrentNode nodePR3V3_8(adcEPS4, ADCChannel_t::CM_07, GAIN_PR_3V3_8, PC_3V3_8_N, true, RANK_PR_3V3_8);
CurrentNode nodePR3V3_9(adcEPS4, ADCChannel_t::CM_06, GAIN_PR_3V3_9, PC_3V3_9_N, true, RANK_PR_3V3_9);
CurrentNode nodePR3V3_10(adcEPS4, ADCChannel_t::CM_05, GAIN_PR_3V3_10, PC_3V3_10_N, true, RANK_PR_3V3_10);
CurrentNode nodePR3V3_11(adcEPS4, ADCChannel_t::CM_04, GAIN_PR_3V3_11, PC_3V3_11_N, true, RANK_PR_3V3_11);
CurrentNode nodePR3V3_12(adcEPS4, ADCChannel_t::CM_03, GAIN_PR_3V3_12, PC_3V3_12_N, true, RANK_PR_3V3_12);
CurrentNode nodePR3V3_PV(adcEPS5, ADCChannel_t::CM_07, GAIN_PR_3V3_PV, PC_3V3_PV_N, true, RANK_PR_3V3_PV);

CurrentNode * nodesPR3V3[COUNT_PR_3V3] = {
  &nodePR3V3_0,
  &nodePR3V3_1,
  &nodePR3V3_2,
  &nodePR3V3_3,
  &nodePR3V3_4,
  &nodePR3V3_5,
  &nodePR3V3_6,
  &nodePR3V3_7,
  &nodePR3V3_8,
  &nodePR3V3_9,
  &nodePR3V3_10,
  &nodePR3V3_11,
  &nodePR3V3_12,
  &nodePR3V3_PV};

CurrentNode nodePRBatt_0(adcEPS4, ADCChannel_t::CM_02, GAIN_PR_BATT_0, PC_BATT_0_N, true, RANK_PR_BATT_0);
CurrentNode nodePRBatt_1(adcEPS4, ADCChannel_t::CM_01, GAIN_PR_BATT_1, PC_BATT_1_N, true, RANK_PR_BATT_1);
CurrentNode nodePRBatt_2(adcEPS4, ADCChannel_t::CM_00, GAIN_PR_BATT_2, PC_BATT_2_N, true, RANK_PR_BATT_2);
CurrentNode nodePRBatt_3(adcEPS5, ADCChannel_t::CM_03, GAIN_PR_BATT_3, PC_BATT_3_N, true, RANK_PR_BATT_3);
CurrentNode nodePRBatt_4(adcEPS5, ADCChannel_t::CM_02, GAIN_PR_BATT_4, PC_BATT_4_N, true, RANK_PR_BATT_4);
CurrentNode nodePRBatt_5(adcEPS5, ADCChannel_t::CM_01, GAIN_PR_BATT_5, PC_BATT_5_N, true, RANK_PR_BATT_5);
CurrentNode nodePRBatt_6(adcEPS5, ADCChannel_t::CM_00, GAIN_PR_BATT_6, PC_BATT_6_N, true, RANK_PR_BATT_6);

CurrentNode * nodesPRBatt[COUNT_PR_BATT] = {
  &nodePRBatt_0,
  &nodePRBatt_1,
  &nodePRBatt_2,
  &nodePRBatt_3,
  &nodePRBatt_4,
  &nodePRBatt_5,
  &nodePRBatt_6};
  
CurrentNode nodeBH_0(adcEPS5, ADCChannel_t::CM_04, GAIN_BH_0, PC_BH_0_N, true);
CurrentNode nodeBH_1(adcEPS5, ADCChannel_t::CM_05, GAIN_BH_1, PC_BH_1_N, true);

CurrentNode * nodesBatteryHeaters[COUNT_BH] = {&nodeBH_0, &nodeBH_1};
  
CurrentNode nodeDeploy_0(adcEPS6, ADCChannel_t::CM_07, GAIN_DEPLOY_0, PC_DEPLOY_0_N, true);
CurrentNode nodeDeploy_1(adcEPS7, ADCChannel_t::CM_00, GAIN_DEPLOY_1, PC_DEPLOY_1_N, true);

CurrentNode * nodesDeployables[COUNT_DEPLOY] = {&nodeDeploy_0, &nodeDeploy_1};
// clang-format on

CurrentNode nodeBattInA(adcEPS2, ADCChannel_t::CM_05, GAIN_BATT_IN_A);
CurrentNode nodeBattInB(adcEPS2, ADCChannel_t::CM_03, GAIN_BATT_IN_B);
CurrentNode nodeBattOutA(adcEPS1, ADCChannel_t::CM_04, GAIN_BATT_OUT_A);
CurrentNode nodeBattOutB(adcEPS2, ADCChannel_t::CM_04, GAIN_BATT_OUT_B);

CurrentNode node3V3InA(adcEPS1, ADCChannel_t::CM_05, GAIN_3V3_IN_A);
CurrentNode node3V3InB(adcEPS0, ADCChannel_t::CM_06, GAIN_3V3_IN_B);
CurrentNode node3V3OutA(adcEPS1, ADCChannel_t::CM_06, GAIN_3V3_OUT_A);
CurrentNode node3V3OutB(adcEPS0, ADCChannel_t::CM_05, GAIN_3V3_OUT_B);

CurrentNode nodePR3V3_EPS(adcEPS5, ADCChannel_t::CM_06, GAIN_PR_3V3_EPS);

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
// clang-format on

// Thermistor thermistorsMPPT[8] = {
//     Thermistor(adcPV0, PIN_ADC_TEMP_MPPT_A, THERM_CAL_PV0_MPPT_A_300K,
//         THERM_CAL_PV0_MPPT_A_340K),
//     Thermistor(adcPV0, PIN_ADC_TEMP_MPPT_B, THERM_CAL_PV0_MPPT_B_300K,
//         THERM_CAL_PV0_MPPT_B_340K),
//     Thermistor(adcPV1, PIN_ADC_TEMP_MPPT_A, THERM_CAL_PV1_MPPT_A_300K,
//         THERM_CAL_PV1_MPPT_A_340K),
//     Thermistor(adcPV1, PIN_ADC_TEMP_MPPT_B, THERM_CAL_PV1_MPPT_B_300K,
//         THERM_CAL_PV1_MPPT_B_340K),
//     Thermistor(adcPV2, PIN_ADC_TEMP_MPPT_A, THERM_CAL_PV2_MPPT_A_300K,
//         THERM_CAL_PV2_MPPT_A_340K),
//     Thermistor(adcPV2, PIN_ADC_TEMP_MPPT_B, THERM_CAL_PV2_MPPT_B_300K,
//         THERM_CAL_PV2_MPPT_B_340K),
//     Thermistor(adcPV3, PIN_ADC_TEMP_MPPT_A, THERM_CAL_PV3_MPPT_A_300K,
//         THERM_CAL_PV3_MPPT_A_340K),
//     Thermistor(adcPV3, PIN_ADC_TEMP_MPPT_B, THERM_CAL_PV3_MPPT_B_300K,
//         THERM_CAL_PV3_MPPT_B_340K)};

// Thermistor thermistorsPVBoard[12] = {
//     Thermistor(adcPV0, PIN_ADC_TEMP_BACK, THERM_CAL_PV0_BACK_300K,
//         THERM_CAL_PV0_BACK_340K),
//     Thermistor(adcPV0, PIN_ADC_TEMP_FRONT_TOP, THERM_CAL_PV0_FRONT_TOP_300K,
//         THERM_CAL_PV0_FRONT_TOP_340K),
//     Thermistor(adcPV0, PIN_ADC_TEMP_FRONT_BOTTOM,
//         THERM_CAL_PV0_FRONT_BOTTOM_300K, THERM_CAL_PV0_FRONT_BOTTOM_340K),
//     Thermistor(adcPV1, PIN_ADC_TEMP_BACK, THERM_CAL_PV1_BACK_300K,
//         THERM_CAL_PV1_BACK_340K),
//     Thermistor(adcPV1, PIN_ADC_TEMP_FRONT_TOP, THERM_CAL_PV1_FRONT_TOP_300K,
//         THERM_CAL_PV1_FRONT_TOP_340K),
//     Thermistor(adcPV1, PIN_ADC_TEMP_FRONT_BOTTOM,
//         THERM_CAL_PV1_FRONT_BOTTOM_300K, THERM_CAL_PV1_FRONT_BOTTOM_340K),
//     Thermistor(adcPV2, PIN_ADC_TEMP_BACK, THERM_CAL_PV1_BACK_300K,
//         THERM_CAL_PV2_BACK_340K),
//     Thermistor(adcPV2, PIN_ADC_TEMP_FRONT_TOP, THERM_CAL_PV2_FRONT_TOP_300K,
//         THERM_CAL_PV2_FRONT_TOP_340K),
//     Thermistor(adcPV2, PIN_ADC_TEMP_FRONT_BOTTOM,
//         THERM_CAL_PV2_FRONT_BOTTOM_300K, THERM_CAL_PV2_FRONT_BOTTOM_340K),
//     Thermistor(adcPV3, PIN_ADC_TEMP_BACK, THERM_CAL_PV3_BACK_300K,
//         THERM_CAL_PV3_BACK_340K),
//     Thermistor(adcPV3, PIN_ADC_TEMP_FRONT_TOP, THERM_CAL_PV3_FRONT_TOP_300K,
//         THERM_CAL_PV3_FRONT_TOP_340K),
//     Thermistor(adcPV3, PIN_ADC_TEMP_FRONT_BOTTOM,
//         THERM_CAL_PV3_FRONT_BOTTOM_300K, THERM_CAL_PV3_FRONT_BOTTOM_340K)};
