/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file ihuPeriodic.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief performs periodic tasks such as getting health statistics
 */
#include "IHU.h"
#include "events.h"

/**
 * Performs periodic tasks such as getting health statistics
 * @param dataBlock unused, pass in nullptr
 * @param dataLength unused, pass in null
 */
void ihuPeriodic(uint8_t *dataBlock, uint16_t dataLength){
  //Get health statistics
  //Adjust operation accordingly
  //Add events if needed
}
