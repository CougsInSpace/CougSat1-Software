/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PowerNodeDirect.h
 * @author Bradley Davis
 * @date 3 Nov 2018
 * @brief A class for a power node consisting of switching, and current
 * monitoring whose switches are connected direct to the processor
 *
 */

#ifndef _SRC_COMPONENTS_POWER_NODE_DIRECT_H_
#define _SRC_COMPONENTS_POWER_NODE_DIRECT_H_

#include "components/PowerNode.h"
#include "drivers/LTC2499.h"
#include "mbed.h"

class PowerNodeDirect : public PowerNode {
public:
  PowerNodeDirect(LTC2499 &adc, LTC2499Channel_t channel, double shunt,
                  PinName switchA, PinName switchB);

  uint8_t setSwitch(bool pathA, bool pathB);

private:
  DigitalOut switchA;
  DigitalOut switchB;
};

#endif /* _SRC_COMPONENTS_POWER_NODE_DIRECT_H_ */