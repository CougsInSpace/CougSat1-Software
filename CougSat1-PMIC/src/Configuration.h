#ifndef _SRC_CONFIGURATION_H_
#define _SRC_CONFIGURATION_H_

/*********************************** Timing ***********************************/
#define PERIOD_MS_IDLE_SLEEP (10) // Sleep time during idle
#define PERIOD_MS_PERIODIC (5000) // The periodic event

/************************************ ADCs ************************************/
#define ADC_AVREF (1.7995)
#define ADC_GAIN_NONE (1.0)
#define ADC_GAIN_EPS5 (0.231)

/******************************** Array Counts ********************************/
#define COUNT_ADC_EPS (5)
#define COUNT_ADC_PV (4)
#define COUNT_PV (8)
#define COUNT_PR_3V3 (13)
#define COUNT_PR_BATT (7)
#define COUNT_PV_3V3 (4)
#define COUNT_BH (2)
#define COUNT_DEPLOYABLES (2)
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

/****** I added this to PMICConfiguration.h **********/
// Comparison Constant for Thermistor readings in celcius

#define THRES_BATT_TEMP_LOW (0) // Lowest Temp allowed by batt
#define THRES_BATT_TEMP_HIGH (50) // highest temp allowed by batt
#define THRES_BATT_MAX_HIGH_TEMP  (75) // used to determine if batt is really HOT
#define ACCEPTABLE_BATT_TEMP (10) // if temp is between 10 C and 50C, we good

#define THRES_REG_TEMP_HIGH (85) // max temp allowed by 3V3 Regulators

#define max_threshold (2.275) // Sum of all the payloads and bus currents
#define temp_threshold (1.1375) // Half of the payloads and buc currents


#define REG_EXPECTED_CURRENT (3)   // current levels of REG3V3 Should be this
#define BATT_EXPECTED_CURRENT (4) // current levels of battery shoul dbe this

// Priority Levels
#define priority_PR3V3_one      100 // Most Important
#define priority_PR3V3_two      101
#define priority_PRBATT_three   102
#define priority_PR3V3_four     103
#define priority_PR3V3_five     104
#define priority_PR3V3_six      105
#define priority_PR3V3_seven    106
#define priority_PRBATT_eight   107
#define priority_PR3V3_nine     108
#define priority_PRBATT_ten     109
#define priority_PR3V3_eleven   110
#define priority_PR3V3_twelve   111
#define priority_PR3V3_thirteen 112
#define priority_PR3V3_fourteen 113
#define priority_PR3V3_fifteen  114 // Least Important

// expected current for payloads in Amps
// Update: Might Not Need these constants
#define PR3V3_one      0.090
#define PR3V3_two      0.105
#define PRBATT_three   0.400
#define PR3V3_four     0.020
#define PR3V3_five     0.060
#define PR3V3_six      0.020
#define PR3V3_seven    0.105
#define PRBATT_eight   0.300
#define PR3V3_nine     0.055
#define PRBATT_ten     0.250
#define PR3V3_eleven   0.050
#define PR3V3_twelve   0.025
#define PR3V3_thirteen 0.015
#define PR3V3_fourteen 0.390
#define PR3V3_fifteen  0.390
#define UNUSED         0

#endif /* _SRC_CONFIGURATION_H_ */
