/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file CISConsole.h
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Communicates between the IHU and the testing computer
 *
 * For use during testing via the umbilical
 * SWO is for debug printing
 * USART is for message printing and recieving
 */

#ifndef SRC_TOOLS_CISCONSOLE_H_
#define SRC_TOOLS_CISCONSOLE_H_

#include "SWO.h"
#include <mbed.h>

// #define NDEBUG
// #define NLOG
// #define NERROR

extern SWO_Channel swo;

/**
 * Prints to the SWO port as follows:
 * "[time stamp] object name: message"
 * @param o name of class or function
 * @param args... standard printf arguments
 */
#ifndef NDEBUG
#define DEBUG(o, args...)                                                      \
  {                                                                            \
    swo.printf("[%07lu][Debug] %-10s: ", HAL_GetTick(), o);                    \
    swo.printf(args);                                                          \
    swo.putc('\n');                                                            \
  }
#else
#define DEBUG(o, ...)                                                          \
  {}
#endif

/**
 * Prints to the SWO port as follows:
 * "[time stamp] object name: message"
 * @param o name of class or function
 * @param args... standard printf arguments
 */
#ifndef NLOG
#define LOG(o, args...)                                                        \
  {                                                                            \
    swo.printf("[%07lu][ Log ] %-10s: ", HAL_GetTick(), o);                    \
    swo.printf(args);                                                          \
    swo.putc('\n');                                                            \
  }
#else
#define LOG(o, ...)                                                            \
  {}
#endif

/**
 * Prints to the SWO port as follows:
 * "[time stamp] object name: message"
 * @param o name of class or function
 * @param args... standard printf arguments
 */
#ifndef NERROR
#define ERROR(o, args...)                                                      \
  {                                                                            \
    swo.printf("[%07lu][Error] %-10s: ", HAL_GetTick(), o);                    \
    swo.printf(args);                                                          \
    swo.putc('\n');                                                            \
  }
#else
#define ERROR(o, ...)                                                          \
  {}
#endif

#endif /* SRC_TOOLS_CISCONSOLE_H_ */
