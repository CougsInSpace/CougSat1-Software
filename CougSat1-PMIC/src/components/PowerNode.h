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

class PowerNode 
{
public:
  PowerNode(LTC2499 & adc, LTC2499Channel_t channel, double shunt, int pNum, double eCurrent);

  uint8_t getCurrent(double * current);

  void    getSwitch(bool * pathA, bool * pathB);

  uint8_t setSwitch(bool pathA, bool pathB);

  uint8_t getVoltage(double * voltage);

  uint8_t getPriority(int priority);

  uint8_t getExpectedCurrent(double current )

  
private:
  LTC2499 & adc;
  LTC2499Channel_t channel;
  double shunt;
  bool   pathA;
  bool   pathB;
  int    priorityN;
  int    expected_Curr;
};

#endif /* _SRC_COMPONENTS_POWER_NODE_H_ */