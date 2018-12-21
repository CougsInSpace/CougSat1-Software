/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PMICPins.h
 * @author Bradley Davis
 * @date 4 Nov 2018
 * @brief Contains all hardware pins as constants
 */

#ifndef _SRC_PMIC_PINS_H_
#define _SRC_PMIC_PINS_H_

/*************************** Backplance Connections ***************************/
#define PIN_CTRL_SYNC (PB_9)

#define PIN_I2C_BUS_SDA (PC_1)
#define PIN_I2C_BUS_SCL (PC_0) // PMIC is a slave on this bus
#define PIN_I2C_BUS_IRQ (PB_8)
#define I2C_ADDR_PMIC ((uint8_t)0x0E)

/******************************** Local Buses *********************************/
#define PIN_I2C_LOCAL_SDA (PB_11) // PMIC is the master on this bus
#define PIN_I2C_LOCAL_SCL (PB_10)
#define I2C_ADDR_ADC_EPS0 ((uint8_t)0x28)  // U15
#define I2C_ADDR_ADC_EPS1 ((uint8_t)0x2A)  // U10
#define I2C_ADDR_ADC_EPS2 ((uint8_t)0x2E)  // U14
#define I2C_ADDR_ADC_EPS3 ((uint8_t)0x2C)  // U12
#define I2C_ADDR_ADC_EPS4 ((uint8_t)0x6A)  // U7
#define I2C_ADDR_ADC_EPS5 ((uint8_t)0x6C)  // U2
#define I2C_ADDR_ADC_PV0 ((uint8_t)0xC8)   // +Z
#define I2C_ADDR_ADC_PV1 ((uint8_t)0xCA)   // -Y
#define I2C_ADDR_ADC_PV2 ((uint8_t)0xCC)   // -X
#define I2C_ADDR_ADC_PV3 ((uint8_t)0xCE)   // +Y
#define I2C_ADDR_GPIO_EPS0 ((uint8_t)0x40) // U3
#define I2C_ADDR_GPIO_EPS1 ((uint8_t)0x42) // U13
#define I2C_ADDR_GPIO_PV0 ((uint8_t)0x44)  // +Z
#define I2C_ADDR_GPIO_PV1 ((uint8_t)0x46)  // -Y
#define I2C_ADDR_GPIO_PV2 ((uint8_t)0x48)  // -X
#define I2C_ADDR_GPIO_PV3 ((uint8_t)0x4A)  // +Y

/**************************** Power Node Switching ****************************/
#define PIN_PV_SW_0A (P06) // GPIO - EPS 0
#define PIN_PV_SW_0B (P07) // GPIO - EPS 0
#define PIN_PV_SW_1A (P10) // GPIO - EPS 0
#define PIN_PV_SW_1B (P11) // GPIO - EPS 0
#define PIN_PV_SW_2A (P15) // GPIO - EPS 0
#define PIN_PV_SW_2B (P13) // GPIO - EPS 0
#define PIN_PV_SW_3A (P12) // GPIO - EPS 0
#define PIN_PV_SW_3B (P14) // GPIO - EPS 0
#define PIN_PV_SW_4A (P04) // GPIO - EPS 1
#define PIN_PV_SW_4B (P05) // GPIO - EPS 1
#define PIN_PV_SW_5A (P06) // GPIO - EPS 1
#define PIN_PV_SW_5B (P10) // GPIO - EPS 1
#define PIN_PV_SW_6A (P07) // GPIO - EPS 1
#define PIN_PV_SW_6B (P12) // GPIO - EPS 1
#define PIN_PV_SW_7A (P11) // GPIO - EPS 1
#define PIN_PV_SW_7B (P13) // GPIO - EPS 1

#define PIN_PVC_3V3_0A (P16) // GPIO - EPS 0
#define PIN_PVC_3V3_0B (P17) // GPIO - EPS 0
#define PIN_PVC_3V3_1A (P04) // GPIO - EPS 0
#define PIN_PVC_3V3_1B (P05) // GPIO - EPS 0
#define PIN_PVC_3V3_2A (P16) // GPIO - EPS 1
#define PIN_PVC_3V3_2B (P17) // GPIO - EPS 1
#define PIN_PVC_3V3_3A (P15) // GPIO - EPS 1
#define PIN_PVC_3V3_3B (P14) // GPIO - EPS 1

#define PIN_UMB_SW_A (PB_6)
#define PIN_UMB_SW_B (PB_7)

#define PIN_PC_3V3_0A (P03) // GPIO - EPS 1
#define PIN_PC_3V3_0B (P02) // GPIO - EPS 1
#define PIN_PC_3V3_1A (P01) // GPIO - EPS 1
#define PIN_PC_3V3_1B (P00) // GPIO - EPS 1
#define PIN_PC_3V3_2A (PC_12)
#define PIN_PC_3V3_2B (PC_11)
#define PIN_PC_3V3_3A (PC_10)
#define PIN_PC_3V3_3B (PA_12)
#define PIN_PC_3V3_4A (PA_11)
#define PIN_PC_3V3_4B (PA_10)
#define PIN_PC_3V3_5A (PA_9)
#define PIN_PC_3V3_5B (PA_8)
#define PIN_PC_3V3_6A (PC_9)
#define PIN_PC_3V3_6B (PC_8)
#define PIN_PC_3V3_7A (PC_7)
#define PIN_PC_3V3_7B (PC_6)
#define PIN_PC_3V3_8A (PB_15)
#define PIN_PC_3V3_8B (PB_14)
#define PIN_PC_3V3_9A (PB_13)
#define PIN_PC_3V3_9B (PB_12)
#define PIN_PC_3V3_10A (PB_2)
#define PIN_PC_3V3_10B (PB_1)
#define PIN_PC_3V3_11A (PB_0)
#define PIN_PC_3V3_11B (PC_5)
#define PIN_PC_3V3_12A (PC_4)
#define PIN_PC_3V3_12B (PA_7)

#define PIN_PC_BATT_0A (PA_6)
#define PIN_PC_BATT_0B (PA_5)
#define PIN_PC_BATT_1A (PA_4)
#define PIN_PC_BATT_1B (PA_3)
#define PIN_PC_BATT_2A (PC_2)
#define PIN_PC_BATT_2B (PC_3)
#define PIN_PC_BATT_3A (PA_0)
#define PIN_PC_BATT_3B (PA_1)
#define PIN_PC_BATT_4A (PA_2)
#define PIN_PC_BATT_4B (PH_1)
#define PIN_PC_BATT_5A (PH_0)
#define PIN_PC_BATT_5B (PC_15)
#define PIN_PC_BATT_6A (PC_14)
#define PIN_PC_BATT_6B (PC_13)

#define PIN_PC_DEPLOY_A (PB_4)
#define PIN_PC_DEPLOY_B (PB_5)

#define PIN_PC_BH_0A (P02) // GPIO - EPS 0
#define PIN_PC_BH_0B (P03) // GPIO - EPS 0
#define PIN_PC_BH_1A (P00) // GPIO - EPS 0
#define PIN_PC_BH_1B (P01) // GPIO - EPS 0

/******************************** ADC Channels ********************************/
#define PIN_ADC_VBATT_I_A (DIFF_4)  // ADC - EPS 4
#define PIN_ADC_VBATT_I_B (DIFF_11) // ADC - EPS 3

#define PIN_ADC_BATT_A (DIFF_13) // ADC - EPS 4
#define PIN_ADC_BATT_B (DIFF_11) // ADC - EPS 4

#define PIN_ADC_3V3_IN_A (DIFF_9)  // ADC - EPS 4
#define PIN_ADC_3V3_IN_B (DIFF_4)  // ADC - EPS 3
#define PIN_ADC_3V3_OUT_A (DIFF_7) // ADC - EPS 4
#define PIN_ADC_3V3_OUT_B (DIFF_6) // ADC - EPS 3

#define PIN_ADC_PR_3V3_0 (DIFF_6)   // ADC - EPS 0
#define PIN_ADC_PR_3V3_1 (DIFF_8)   // ADC - EPS 0
#define PIN_ADC_PR_3V3_2 (DIFF_5)   // ADC - EPS 0
#define PIN_ADC_PR_3V3_3 (DIFF_12)  // ADC - EPS 0
#define PIN_ADC_PR_3V3_4 (DIFF_3)   // ADC - EPS 0
#define PIN_ADC_PR_3V3_5 (DIFF_14)  // ADC - EPS 0
#define PIN_ADC_PR_3V3_6 (DIFF_1)   // ADC - EPS 0
#define PIN_ADC_PR_3V3_7 (DIFF_10)  // ADC - EPS 0
#define PIN_ADC_PR_3V3_8 (DIFF_9)   // ADC - EPS 2
#define PIN_ADC_PR_3V3_9 (DIFF_10)  // ADC - EPS 2
#define PIN_ADC_PR_3V3_10 (DIFF_14) // ADC - EPS 2
#define PIN_ADC_PR_3V3_11 (DIFF_12) // ADC - EPS 2
#define PIN_ADC_PR_3V3_12 (DIFF_10) // ADC - EPS 1

#define PIN_ADC_PR_BATT_0 (DIFF_14) // ADC - EPS 1
#define PIN_ADC_PR_BATT_1 (DIFF_4)  // ADC - EPS 1
#define PIN_ADC_PR_BATT_2 (DIFF_0)  // ADC - EPS 1
#define PIN_ADC_PR_BATT_3 (DIFF_9)  // ADC - EPS 1
#define PIN_ADC_PR_BATT_4 (DIFF_2)  // ADC - EPS 1
#define PIN_ADC_PR_BATT_5 (DIFF_7)  // ADC - EPS 1
#define PIN_ADC_PR_BATT_6 (DIFF_13) // ADC - EPS 1

#define PIN_ADC_PV_3V3_0 (DIFF_1)  // ADC - EPS 4
#define PIN_ADC_PV_3V3_1 (DIFF_15) // ADC - EPS 4
#define PIN_ADC_PV_3V3_2 (DIFF_1)  // ADC - EPS 2
#define PIN_ADC_PV_3V3_3 (DIFF_3)  // ADC - EPS 2

#define PIN_ADC_PR_DEPLOY (DIFF_8) // ADC - EPS 3

#define PIN_ADC_PR_BH_0 (DIFF_2)  // ADC - EPS 4
#define PIN_ADC_PR_BH_1 (DIFF_13) // ADC - EPS 3

#define PIN_ADC_TEMP_BATT_A (SINGLE_0) // ADC - EPS 5
#define PIN_ADC_TEMP_BATT_B (SINGLE_3) // ADC - EPS 5
#define PIN_ADC_TEMP_PMIC (SINGLE_2)   // ADC - EPS 5
#define PIN_ADC_TEMP_REG_A (SINGLE_6)  // ADC - EPS 5
#define PIN_ADC_TEMP_REG_B (SINGLE_14) // ADC - EPS 5
#define PIN_ADC_TEMP_pXpY (SINGLE_13)  // ADC - EPS 5
#define PIN_ADC_TEMP_nXpY (SINGLE_5)   // ADC - EPS 5
#define PIN_ADC_TEMP_pXnY (SINGLE_9)   // ADC - EPS 5
#define PIN_ADC_TEMP_nXnY (SINGLE_10)  // ADC - EPS 5
#define PIN_ADC_AVREF (SINGLE_1)       // ADC - EPS 5
#define PIN_ADC_UMB_IN (SINGLE_4)      // ADC - EPS 5
#define PIN_ADC_EJECT_TIMER (SINGLE_8) // ADC - EPS 5
#define PIN_ADC_VBATT_A (SINGLE_7)     // ADC - EPS 5
#define PIN_ADC_VBATT_B (SINGLE_15)    // ADC - EPS 5
#define PIN_ADC_3V3_A (SINGLE_11)      // ADC - EPS 5
#define PIN_ADC_3V3_B (SINGLE_12)      // ADC - EPS 5

/******************************** Solar Panel *********************************/
// Each panel has the same pins but different addresses to the I²C devices
#define PIN_MPPT_XSHUT_A (P01) // GPIO - PV0, PV1, PV2, PV3
#define PIN_MPPT_XSHUT_B (P00) // GPIO - PV0, PV1, PV2, PV3

#define PIN_ADC_PVIN_I_A (DIFF_3)  // ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_PVIN_I_B (DIFF_4)  // ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_PVOUT_I_A (DIFF_1) // ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_PVOUT_I_B (DIFF_9) // ADC - PV0, PV1, PV2, PV3

#define PIN_ADC_PVIN_A (SINGLE_3)             //ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_PVIN_B (SINGLE_4)             //ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_PVOUT_A (SINGLE_1)            //ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_PVOUT_B (SINGLE_9)            //ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_TEMP_MPPT_A (SINGLE_12)       //ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_TEMP_MPPT_B (SINGLE_13)       //ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_TEMP_BACK (SINGLE_6)          //ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_TEMP_FRONT_TOP (SINGLE_15)    //ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_TEMP_FRONT_BOTTOM (SINGLE_14) //ADC - PV0, PV1, PV2, PV3
#define PIN_ADC_PV_AVREF (SINGLE_7)           //ADC - PV0, PV1, PV2, PV3

/******************************* Miscellaneous ********************************/
#define PIN_LED (PD_2)

#endif /* _SRC_PMIC_PINS_H_ */
