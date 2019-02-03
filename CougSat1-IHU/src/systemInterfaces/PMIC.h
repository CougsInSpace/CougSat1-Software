/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PMIC.h
 * @author Bradley Davis, Kevin Evans
 * @date 3 Feb 2019
 * @brief Communicates betwen the IHU and the PMIC
 */
#ifndef SRC_SYSTEMINTERFACES_PMIC_H

#define SRC_SYSTEMINTERFACES_PMIC_H

#define I2C_ADDR_PMIC           0x0E

#include <mbed.h>
#include <systemInterfaces/Subsystem.h>

class PMIC : public SubSystem {
  public:
    // Command IDs sent over I2C to the PMIC.
    typedef enum PMICCommand {
      REQ_SUBSYSTEM_OFF,
      REQ_SUBSYSTEM_ON,
      REQ_VOLTAGE_DATA,
      REQ_CURRENT_DATA,
      REQ_TEMPERATURE_DATA,
      REQ_POWER_CHANNEL_STATUS,
      REQ_SOLAR_PANEL_CHANNEL_STATUS
    } PMICCommand_t;

    typedef enum TargetSubSystem {
      // Used for toggling power:
      IHU,
      IFJR,
      ADCS,
      ADCS_COILS,
      COMMS,
      COMMS_AMP,
      PAYLOAD_0,
      PAYLOAD_1,
      PAYLOAD_2
    } TargetSubSystem_t;

    typedef enum TargetReading {
      // Used for voltage, reading, and temperature:
      SOLAR_PANEL_0,
      SOLAR_PANEL_1,
      SOLAR_PANEL_2, 
      SOLAR_PANEL_3,
      SOLAR_PANEL_4,
      SOLAR_PANEL_5,
      SOLAR_PANEL_6,
      SOLAR_PANEL_7,
      BATTERY_A,
      BATTERY_B,
      REGULATOR_A_3V3,
      REGULATOR_B_3V3,

      // Used for current only:
      PR_3V3_0,
      PR_3V3_1,
      PR_3V3_2,
      PR_3V3_3,
      PR_3V3_4,
      PR_3V3_5,
      PR_3V3_6,
      PR_3V3_7,
      PR_3V3_8,
      PR_3V3_9,
      PR_3V3_10,
      PR_3V3_11,
      PR_3V3_12,
      PR_BATT_0,
      PR_BATT_1,
      PR_BATT_2,
      PR_BATT_3,
      PR_BATT_4,
      PR_BATT_5,
      PR_BATT_6,
      PV_3V3_0,
      PV_3V3_1,
      PV_3V3_2,
      PV_3V3_3,
      PR_BH_0,
      PR_BH_1,
      PR_DEPLOY,

      // Used for temperature readings:
      TARGET_PMIC,
      MPPT_0,
      MPPT_1,
      MPPT_2,
      MPPT_3,
      MPPT_4,
      MPPT_5,
      MPPT_6,
      MPPT_7,
      PCB_NX_NY,
      PCB_NX_PY,
      PCB_PX_NY,
      PCB_PX_PY
    } TargetReading_t;

    PMIC(I2C &i2c);
    ~PMIC();

    uint8_t initialize();
    uint8_t requestSetSubSystemPower(TargetSubSystem_t target, bool on, uint8_t *result);
    uint8_t requestGetVoltageData(TargetReading_t target, uint8_t *output);
    uint8_t requestGetCurrentData(TargetReading_t target, uint8_t *output);
    uint8_t requestGetTemperatureData(TargetReading_t target, int8_t *temperature);
    uint8_t requestGetPowerChannelStatus(uint64_t *status);
    uint8_t requestGetSolarPanelChannelStatus(uint16_t *status);
    
  private:
    I2C &i2c;

};

#endif /* !SRC_SYSTEMINTERFACES_PMIC_H */
