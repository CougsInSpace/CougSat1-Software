/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file RCS.h
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates betwen the IHU and the RCS
 */

#ifndef SRC_SYSTEMINTERFACES_RCS_H_

#define SRC_SYSTEMINTERFACES_RCS_H_

#include <mbed.h>
#include <systemInterfaces/Subsystem.h>

class RCS : public SubSystem {
  public:
    RCS();
    ~RCS();

    uint8_t initialize();

  private:

};

#endif /* !SRC_SYSTEMINTERFACES_RCS_H_ */
