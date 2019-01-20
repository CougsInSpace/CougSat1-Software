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
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates betwen the IHU and the PMIC
 */
#ifndef SRC_SYSTEMINTERFACES_PMIC_H

#define SRC_SYSTEMINTERFACES_PMIC_H

#define I2C_ADDR_PMIC 0x0E

#include <mbed.h>
#include <systemInterfaces/Subsystem.h>

class PMIC : public SubSystem {
  public:
    enum TargetSubsystem {
      IHU       = 0x00,
      IFJR      = 0x01,
      ADCS      = 0x02,
      ADCSCoils = 0x03,
      Comms     = 0x04,
      CommsAmp  = 0x05,
      Payload0  = 0x06,
      Payload1  = 0x07,
      Payload2  = 0x08
    };
    enum TargetReading {
      // voltage/current:
      SolarPanel0 = 0x00,
      SolarPanel1,
      SolarPanel2, 
      SolarPanel3,
      SolarPanel4,
      SolarPanel5,
      SolarPanel6,
      SolarPanel7,
      BatteryA,
      BatteryB,
      RegulatorA3V3,
      RegulatorB3V3,

      // current:
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

      // for temp readings:
      TargetPMIC,
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
    };

    PMIC(I2C &i2c);
    ~PMIC();

    uint8_t initialize();
    uint8_t requestSetSubSystemPower(PMIC::TargetSubsystem target, bool on);
    uint8_t requestGetVoltageData(PMIC::TargetReading target);
    uint8_t requestGetCurrentData(PMIC::TargetReading target);
    uint8_t requestGetTempData(PMIC::TargetReading target);

  private:
    I2C &i2c;

};

#endif /* !SRC_SYSTEMINTERFACES_PMIC_H */
