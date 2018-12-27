/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PMICConfiguration.h
 * @author Bradley Davis
 * @date 4 Nov 2018
 * @brief Contains all configuration settings
 *
 * Resistances of shunts, themistor calibration points, and parameter thresholds
 */

#ifndef _SRC_PMIC_CONFIGURATION_H_
#define _SRC_PMIC_CONFIGURATION_H_

/*********************************** Timing ***********************************/
#define PERIOD_MS_IDLE_SLEEP (10)  // Sleep time during idle
#define PERIOD_MS_ADC_UPDATE (150) // Read next active channels on ADCs
#define PERIOD_MS_PERIODIC (5000)  // The periodic event

/******************************** Array Counts ********************************/
#define COUNT_PV (8)
#define COUNT_PR_3V3 (13)
#define COUNT_PR_BATT (7)
#define COUNT_PV_3V3 (4)
#define COUNT_BH (2)
#define COUNT_THERM_MPPT (8)
#define COUNT_THERM_BOARD (12)

/********************************* Thresholds *********************************/
#define THRES_EJECT_TIMER (2.7) // 1 RC with 4.2V source, 2 RC with 3.0V source

/******************************** Solar Panel *********************************/
#define SHUNT_PVIN_0A (0.05)
#define SHUNT_PVIN_0B (0.05)
#define SHUNT_PVIN_1A (0.05)
#define SHUNT_PVIN_1B (0.05)
#define SHUNT_PVIN_2A (0.05)
#define SHUNT_PVIN_2B (0.05)
#define SHUNT_PVIN_3A (0.05)
#define SHUNT_PVIN_3B (0.05)

#define SHUNT_PVOUT_0A (0.10)
#define SHUNT_PVOUT_0B (0.10)
#define SHUNT_PVOUT_1A (0.10)
#define SHUNT_PVOUT_1B (0.10)
#define SHUNT_PVOUT_2A (0.10)
#define SHUNT_PVOUT_2B (0.10)
#define SHUNT_PVOUT_3A (0.10)
#define SHUNT_PVOUT_3B (0.10)

#define SHUNT_PV_3V3_0 (0.05)
#define SHUNT_PV_3V3_1 (0.05)
#define SHUNT_PV_3V3_2 (0.05)
#define SHUNT_PV_3V3_3 (0.05)

/******************************** Power Rails *********************************/
#define SHUNT_PR_3V3_0 (0.05)
#define SHUNT_PR_3V3_1 (0.05)
#define SHUNT_PR_3V3_2 (0.05)
#define SHUNT_PR_3V3_3 (0.05)
#define SHUNT_PR_3V3_4 (0.05)
#define SHUNT_PR_3V3_5 (0.05)
#define SHUNT_PR_3V3_6 (0.05)
#define SHUNT_PR_3V3_7 (0.05)
#define SHUNT_PR_3V3_8 (0.05)
#define SHUNT_PR_3V3_9 (0.05)
#define SHUNT_PR_3V3_10 (0.05)
#define SHUNT_PR_3V3_11 (0.05)
#define SHUNT_PR_3V3_12 (0.05)

#define SHUNT_PR_BATT_0 (0.05)
#define SHUNT_PR_BATT_1 (0.05)
#define SHUNT_PR_BATT_2 (0.05)
#define SHUNT_PR_BATT_3 (0.05)
#define SHUNT_PR_BATT_4 (0.05)
#define SHUNT_PR_BATT_5 (0.05)
#define SHUNT_PR_BATT_6 (0.05)

#define SHUNT_BH_0 (0.05)
#define SHUNT_BH_1 (0.05)

#define SHUNT_DEPLOYABLES (0.025)

/******************************** Power Chain *********************************/
#define SHUNT_VBATT_A (0.0133)
#define SHUNT_VBATT_B (0.0133)
#define SHUNT_BATT_A (0.01)
#define SHUNT_BATT_B (0.01)
#define SHUNT_3V3_IN_A (0.02)
#define SHUNT_3V3_IN_B (0.02)
#define SHUNT_3V3_OUT_A (0.01)
#define SHUNT_3V3_OUT_B (0.01)

/******************************** Thermistors *********************************/
#define THERM_CAL_BATT_A_270K (0.158)
#define THERM_CAL_BATT_A_350K (0.852)
#define THERM_CAL_BATT_B_270K (0.158)
#define THERM_CAL_BATT_B_350K (0.852)
#define THERM_CAL_PMIC_270K (0.158)
#define THERM_CAL_PMIC_350K (0.852)
#define THERM_CAL_REG_A_270K (0.158)
#define THERM_CAL_REG_A_350K (0.852)
#define THERM_CAL_REG_B_270K (0.158)
#define THERM_CAL_REG_B_350K (0.852)
#define THERM_CAL_pXpY_270K (0.158)
#define THERM_CAL_pXpY_350K (0.852)
#define THERM_CAL_nXpY_270K (0.158)
#define THERM_CAL_nXpY_350K (0.852)
#define THERM_CAL_pXnY_270K (0.158)
#define THERM_CAL_pXnY_350K (0.852)
#define THERM_CAL_nXnY_270K (0.158)
#define THERM_CAL_nXnY_350K (0.852)

#define THERM_CAL_PV0_MPPT_A_270K (0.158)
#define THERM_CAL_PV0_MPPT_A_350K (0.852)
#define THERM_CAL_PV0_MPPT_B_270K (0.158)
#define THERM_CAL_PV0_MPPT_B_350K (0.852)
#define THERM_CAL_PV0_BACK_270K (0.158)
#define THERM_CAL_PV0_BACK_350K (0.852)
#define THERM_CAL_PV0_FRONT_TOP_270K (0.158)
#define THERM_CAL_PV0_FRONT_TOP_350K (0.852)
#define THERM_CAL_PV0_FRONT_BOTTOM_270K (0.158)
#define THERM_CAL_PV0_FRONT_BOTTOM_350K (0.852)

#define THERM_CAL_PV1_MPPT_A_270K (0.158)
#define THERM_CAL_PV1_MPPT_A_350K (0.852)
#define THERM_CAL_PV1_MPPT_B_270K (0.158)
#define THERM_CAL_PV1_MPPT_B_350K (0.852)
#define THERM_CAL_PV1_BACK_270K (0.158)
#define THERM_CAL_PV1_BACK_350K (0.852)
#define THERM_CAL_PV1_FRONT_TOP_270K (0.158)
#define THERM_CAL_PV1_FRONT_TOP_350K (0.852)
#define THERM_CAL_PV1_FRONT_BOTTOM_270K (0.158)
#define THERM_CAL_PV1_FRONT_BOTTOM_350K (0.852)

#define THERM_CAL_PV2_MPPT_A_270K (0.158)
#define THERM_CAL_PV2_MPPT_A_350K (0.852)
#define THERM_CAL_PV2_MPPT_B_270K (0.158)
#define THERM_CAL_PV2_MPPT_B_350K (0.852)
#define THERM_CAL_PV2_BACK_270K (0.158)
#define THERM_CAL_PV2_BACK_350K (0.852)
#define THERM_CAL_PV2_FRONT_TOP_270K (0.158)
#define THERM_CAL_PV2_FRONT_TOP_350K (0.852)
#define THERM_CAL_PV2_FRONT_BOTTOM_270K (0.158)
#define THERM_CAL_PV2_FRONT_BOTTOM_350K (0.852)

#define THERM_CAL_PV3_MPPT_A_270K (0.158)
#define THERM_CAL_PV3_MPPT_A_350K (0.852)
#define THERM_CAL_PV3_MPPT_B_270K (0.158)
#define THERM_CAL_PV3_MPPT_B_350K (0.852)
#define THERM_CAL_PV3_BACK_270K (0.158)
#define THERM_CAL_PV3_BACK_350K (0.852)
#define THERM_CAL_PV3_FRONT_TOP_270K (0.158)
#define THERM_CAL_PV3_FRONT_TOP_350K (0.852)
#define THERM_CAL_PV3_FRONT_BOTTOM_270K (0.158)
#define THERM_CAL_PV3_FRONT_BOTTOM_350K (0.852)

#endif /* _SRC_PMIC_CONFIGURATION_H_ */
