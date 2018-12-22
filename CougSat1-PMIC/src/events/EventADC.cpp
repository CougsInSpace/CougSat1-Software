/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file EventADC.cpp
 * @author Bradley Davis
 * @date 7 Nov 2018
 * @brief Event to handle updating the ADCs
 *
 */
#include "Events.h"
#include "tools/CISConsole.h"
#include "tools/CISError.h"

/**
 * @brief Reads the next channel in each ADC queue
 *
 * @return uint8_t error code
 */
uint8_t eventADC() {
  return ERROR_NOT_SUPPORTED;
}