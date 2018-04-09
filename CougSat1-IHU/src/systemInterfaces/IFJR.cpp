/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file IFJR.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates between the IHU and the IFJR
 */

#include "IFJR.h"
#include "tools/CISError.h"

/**
 * Constructs IFJR object
 */
IFJR::IFJR() {
}

/**
 * Destructor is currently empty
 */
IFJR::~IFJR() {
}

/**
 * Initializes interface to IFJR
 * @return error code
 */
uint8_t IFJR::initialize(){
  return ERROR_SUCCESS;
}
