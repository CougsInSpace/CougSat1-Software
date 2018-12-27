/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file EventFirstBoot.cpp
 * @author Bradley Davis
 * @date 25 Dec 2018
 * @brief Event to handle the first boot sequence
 *
 */
#include "Events.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

/**
 * @brief Initializes hardware and timers into first boot configuration
 *
 * @return uint8_t error code
 */
uint8_t eventFirstBoot() {
  return ERROR_NOT_SUPPORTED;
}