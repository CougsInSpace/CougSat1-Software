/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file CISConsole.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates between the IHU and the testing computer
 *
 * For use during testing via the umbilical
 * SWO is for debug printing
 * USART is for message printing and recieving
 */

#include "CISConsole.h"

SWO_Channel swo;

