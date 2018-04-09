/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file ADCS.h
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates between the IHU and the ADCS
 */

#ifndef SRC_SYSTEMINTERFACES_ADCS_H_

#define SRC_SYSTEMINTERFACES_ADCS_H_

#include <mbed.h>
#include <systemInterfaces/Subsystem.h>

#define I2C_ADDR_ADCS 0xAC

class ADCS : public SubSystem {
  public:
    ADCS(I2C &i2c);
    ~ADCS();

    uint8_t initialize();

  private:
    I2C &i2c;

};

#endif /* !SRC_SYSTEMINTERFACES_ADCS_H_ */
