/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file ADCS.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates betwen the IHU and the ADCS
 */

#include "ADCS.h"
#include "tools/CISError.h"

/**
 * Constructs ADCS object
 * Saves a reference to the i2c
 * @param i2c connected to ADCS
 */
ADCS::ADCS(I2C &i2c) : i2c(i2c) {
}

/**
 * Destructor is currently empty
 */
ADCS::~ADCS() {

}

/**
 * Initializes interface to ADCS
 * @return error code
 */
uint8_t ADCS::initialize(){

  return ERROR_SUCCESS;
}
