/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PowerNodeTCA9535.h
 * @author Bradley Davis
 * @date 3 Nov 2018
 * @brief A class for a power node consisting of switching, and current
 * monitoring whose switches are connected to a TCA9535 GPIO expander
 *
 */

#ifndef _SRC_COMPONENTS_POWER_NODE_TCA9535_H_
#define _SRC_COMPONENTS_POWER_NODE_TCA9535_H_

#include "components/PowerNode.h"
#include "drivers/TCA9535.h"
#include "mbed.h"

class PowerNodeTCA9535 : public PowerNode {
public:
  PowerNodeTCA9535(LTC2499 &adc, LTC2499Channel_t channel, double shunt,
                   TCA9535 &gpio, GPIOExpanderPin_t switchA,
                   GPIOExpanderPin_t switchB);

  uint8_t setSwitch(bool pathA, bool pathB);

private:
  TCA9535 &gpio;
  GPIOExpanderPin_t switchA;
  GPIOExpanderPin_t switchB;
};

#endif /* _SRC_COMPONENTS_POWER_NODE_TCA9535_H_ */