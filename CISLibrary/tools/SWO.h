/* mbed SWO Library
 *  Copyright (c) 2014, v01: WH. Ported from Segger example
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

#ifndef MBED_SWO_H
#define MBED_SWO_H

#include "mbed.h"

//
// This is the Class implementation
//

/**
 * @code
 * #include "mbed.h"
 * #include "SWO.h"
 *
 * DigitalOut myled(LED1);
 *
 * Serial pc(SERIAL_TX, SERIAL_RX);
 *
 * SWO_Channel SWO();
 *
 * int main() {
 *   pc.printf("Hello World\n\r");
 *
 *   SWO.printf("\r\nHello World from SWO\r\n");
 *   SWO.printf("CPU SystemCoreClock is %d Hz\r\n", SystemCoreClock);
 *
 *   while(1) {
 *     myled = 1; // LED is ON
 *     wait(0.2); // 200 ms
 *     myled = 0; // LED is OFF
 *     wait(1.0); // 1 sec
 *
 *     SWO.putc('#');
 *   }
 * }
 * @endcode
 */

//#define DEBUG_FULL

/** An SWO interface for debugging that supports Stream
 *
 * @brief Currently works on nucleo ST-LINK using ST-Link Utility and other devices that support SWD/SWO using Segger SWO viewer
 *
 */
class SWO_Channel : public Stream {

public:
  /** Create an SWO interface for debugging that supports Stream
   *
   */
  SWO_Channel();

#if DOXYGEN_ONLY
  /** Write a character to the display
    *
    * @param c The character to write to the display
    */
  int putc(int c);

  /** Write a formatted string to the display
    *
    * @param format A printf-style format string, followed by the
    *               variables to use in formatting the string.
    */
  int printf(const char* format, ...);
#endif

protected:
  // Stream implementation functions
  virtual int _putc(int value);
  virtual int _getc();

private:

};


//
//This is the classic implementation
//

/**
 * @code
 * #include "mbed.h"
 * #include "SWO.h"
 *
 * DigitalOut myled(LED1);
 *
 * Serial pc(SERIAL_TX, SERIAL_RX);
 *
 * int main() {
 *   pc.printf("Hello World\n\r");
 *
 *   SWO_PrintString("\r\nHello World from SWO\r\n");
 *   char message[64];
 *   sprintf(message, "CPU SystemCoreClock is %d Hz\r\n", SystemCoreClock);
 *   SWO_PrintString(message);
 *
 *   while(1) {
 *     myled = 1; // LED is ON
 *     wait(0.2); // 200 ms
 *     myled = 0; // LED is OFF
 *     wait(1.0); // 1 sec
 *
 *     SWO_PrintString("#");
 *   }
 * }
 * @endcode
 */

// Prototypes

/**
* @brief
*   Checks if SWO is set up. If it is not, return,
*    to avoid program hangs if no debugger is connected.
*   If it is set up, print a character to the ITM_STIM register
*    in order to provide data for SWO.
* @param c The Character to be printed.
* @notes   Additional checks for device specific registers can be added.
*/
void SWO_PrintChar  (char c);


/**
*
* SWO_PrintString()
*
* @brief Print a string via SWO.
* @param *s The string to be printed.
*/
void SWO_PrintString(const char *s);

#endif
