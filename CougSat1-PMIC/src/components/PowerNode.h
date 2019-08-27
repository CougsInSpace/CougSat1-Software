/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PowerNode.h
 * @author Bradley Davis
 * @date 3 Nov 2018
 * @brief A class for a power node consisting of switching, and current
 * monitoring
 *
 */

#ifndef _SRC_COMPONENTS_POWER_NODE_H_
#define _SRC_COMPONENTS_POWER_NODE_H_

#include "drivers/LTC2499.h"
#include "mbed.h"

class PowerNode {
public:
  PowerNode(LTC2499 & adc, LTC2499::ADCChannel_t channel, double shunt);

  uint8_t getCurrent(double * current);

  void getSwitch(bool * pathA, bool * pathB);

  virtual uint8_t setSwitch(bool pathA, bool pathB);

protected:
  bool pathA;
  bool pathB;
  bool inverted;

private:
  LTC2499 & adc;

  LTC2499::ADCChannel_t channel;

  double shunt;
};

#endif /* _SRC_COMPONENTS_POWER_NODE_H_ */