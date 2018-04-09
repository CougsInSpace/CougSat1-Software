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

#include <mbed.h>
#include <systemInterfaces/Subsystem.h>

class PMIC : public SubSystem {
  public:
    PMIC(I2C &i2c);
    ~PMIC();

    uint8_t initialize();

  private:
    I2C &i2c;

};

#endif /* !SRC_SYSTEMINTERFACES_PMIC_H */
