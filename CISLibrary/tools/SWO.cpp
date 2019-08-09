/* mbed SWO Library
 *  Copyright (c) 2014, v01: WH. Ported from Segger example (www.segger.com)
 *                      v02: WH. Added Class with Stream support
 *
 * Simple implementation for tracing via Serial Wire Output(SWO) for Cortex-M processors.
 * It can be used with Host PC software such as ST-LINK Utility or Segger J-Link SWO viewer.
 * This sample implementation ensures that output via SWO is enabled in order to guarantee
 * that the application does not hang.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "mbed.h"
#include "SWO.h"

//
// This the Class implementation
//

/** Create and SWO interface for debugging that supports Stream
  * @brief Currently works on nucleo ST-LINK using ST-Link Utility and other devices that support SWD/SWO using Segger SWO viewer
  */
SWO_Channel::SWO_Channel () {
  //May want to add initialisation stuff here
}

/** Write a single character (Stream implementation)
  *
  * @param value character to be displayed
  * @return value
  */
int SWO_Channel::_putc(int value) {

  //Use CMSIS_core_DebugFunctions. See core_cm3.h
  ITM_SendChar(value);

  return value;
}

/** Get a single character (Stream implementation)
  * @return -1 Not supported
  */
int SWO_Channel::_getc() {
    return -1;
}


//
//This is the classic implementation
//

/**
 * Defines for Cortex-M debug unit
 */
#define ITM_STIM_U32(n) (*(volatile unsigned int*) (0xE0000000+4*n))  // Stimulus Port n Register word access
#define ITM_STIM_U8(n)  (*(volatile unsigned char*)(0xE0000000+4*n))  // Stimulus Port n Register byte access
//#define ITM_STIM_U32_0  (*(volatile unsigned int*)0xE0000000)        // Stimulus Port 0 Register word access
//#define ITM_STIM_U8_0   (*(volatile         char*)0xE0000000)        // Stimulus Port 0 Register byte access
#define ITM_ENA         (*(volatile unsigned int*)0xE0000E00)        // Trace Enable Ports Register
#define ITM_TCR         (*(volatile unsigned int*)0xE0000E80)        // Trace control register

#define ITM_STIM_FIFOREADY                        0x00000001         // FIFO empty

//Stuff below is for documentation and needs further testing
// It seems that the Segger SWO Viewer and the ST-Link Utility do most/all of these
// initialisations on the target before starting the session. This is probably not the case
// when using GDB/OpenOCD.
//
//
#if(0)
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <libopencm3/stm32/dbgmcu.h>
#include <libopencm3/cm3/scs.h>
#include <libopencm3/cm3/tpiu.h>
#include <libopencm3/cm3/itm.h>

/**
 * SWO_Setup() Example
 *
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * https://github.com/1divf/libopenstm32/blob/master/examples/stm32/stm32-h103/traceswo/traceswo.c
 *
 */
void SWO_Setup(void) {
    /* Enable trace subsystem (we'll use ITM and TPIU) */
    SCS_DEMCR |= SCS_DEMCR_TRCENA;

    /* Use Manchester code for asynchronous transmission */
    TPIU_SPPR = TPIU_SPPR_ASYNC_MANCHESTER;
    TPIU_ACPR = 7;

    /* Data width is 1 byte */
    TPIU_CSPSR = TPIU_CSPSR_BYTE;

    /* Formatter and flush control */
    TPIU_FFCR &= ~TPIU_FFCR_ENFCONT;

    /* Enable TRACESWO pin for async mode */
    DBGMCU_CR = DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE_ASYNC;

    /* Unlock access to ITM registers */
    /* FIXME: Magic numbers... Is this Cortex-M3 generic? */
    *((volatile uint32_t*)0xE0000FB0) = 0xC5ACCE55;

    /* Enable ITM with ID = 1 */
    ITM_TCR = (1 << 16) | ITM_TCR_ITMENA;
    /* Enable stimulus port 1 */
    ITM_TER[0] = 1;
}

/**
 * SWO_Setup() Example
 *
 * http://forum.segger.com/index.php?page=Thread&threadID=608
 *
 */
void SWO_Setup_1(void) {
  U32 SWOPrescaler;
  U32 SWOSpeed;

//<Init PLL, set CPU clock to 72 MHz>  // Optional, so I do not pos it here

  SWOSpeed = 6000000;
   *((volatile unsigned *)0xE000EDFC) = 0x01000000;   // "Debug Exception and Monitor Control Register (DEMCR)"
   *((volatile unsigned *)0xE0042004) = 0x00000027;
   *((volatile unsigned *)0xE00400F0) = 0x00000002;   // "Selected PIN Protocol Register": Select which protocol to use for trace output (2: SWO)
  SWOPrescaler = (72000000 / SWOSpeed) - 1;  // SWOSpeed in Hz
   *((volatile unsigned *)0xE0040010) = SWOPrescaler; // "Async Clock Prescaler Register". Scale the baud rate of the asynchronous output
   *((volatile unsigned *)0xE0000FB0) = 0xC5ACCE55;   // ITM Lock Access Register, C5ACCE55 enables more write access to Control Register 0xE00 :: 0xFFC
   *((volatile unsigned *)0xE0000E80) = 0x0001000D;   // ITM Trace Control Register
   *((volatile unsigned *)0xE0000E40) = 0x0000000F;   // ITM Trace Privilege Register
   *((volatile unsigned *)0xE0000E00) = 0x00000001;   // ITM Trace Enable Register. Enabled tracing on stimulus ports. One bit per stimulus port.
   *((volatile unsigned *)0xE0001000) = 0x400003FE;   // DWT_CTRL
   *((volatile unsigned *)0xE0040304) = 0x00000100;   // Formatter and Flush Control Register
}
#endif

/**
 * SWO_PrintChar()
 *
 * @brief
 *   Checks if SWO is set up. If it is not, return,
 *    to avoid program hangs if no debugger is connected.
 *   If it is set up, print a character to the ITM_STIM register
 *    in order to provide data for SWO.
 * @param c The Character to be printed.
 * @notes   Additional checks for device specific registers can be added.
 */
void SWO_PrintChar(char c) {

#if(1)
//Use CMSIS_core_DebugFunctions. See core_cm3.h
  ITM_SendChar (c);

#else
//Use Segger example. Basically same as CMSIS

  // Check if ITM_TCR.ITMENA is set
  if ((ITM_TCR & 1) == 0) {
    return;
  }

  // Check if stimulus port is enabled
  if ((ITM_ENA & 1) == 0) {
    return;
  }

  // Wait until STIMx FIFO is ready, then send data
//  while ((ITM_STIM_U8(0) & 1) == 0);
  while (!(ITM_STIM_U8(0) & ITM_STIM_FIFOREADY));
  ITM_STIM_U8(0) = c;

//  while ((ITM_STIM_U32(0) & 1) == 0);
//  ITM_STIM_U32(0) = c;

#endif
}

/**
 * SWO_PrintString()
 *
 * @brief Print a string via SWO.
 * @param *s The string to be printed.
 *
 */
void SWO_PrintString(const char *s) {

  // Print out characters until \0
  while (*s) {
    SWO_PrintChar(*s++);
  }
}



