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
 * @dat 4 Nov 2018
 * @brief The PMIC class representing the PMIC processor
 *
 */

#ifndef _SRC_PMIC_H_
#define _SRC_PMIC_H_

#include "PMICPins.h"
#include "drivers/LTC2499.h"
#include "mbed.h"

class PMIC {
public:
  PMIC();
  uint8_t initialize();
  uint8_t run();
  uint8_t stop();

private:
};

#endif /* _SRC_PMIC_H_ */