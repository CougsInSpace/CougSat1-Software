/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file RCS.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates betwen the IHU and the RCS
 */

#include "RCS.h"
#include "tools/CISError.h"

/**
 * Creates RCS object
 */
RCS::RCS() {

}

/**
 * Destructor is currently empty
 */
RCS::~RCS() {

}

/**
 * Initializes interface to RCS
 * @return error code
 */
uint8_t RCS::initialize(){
  return ERROR_SUCCESS;
}
