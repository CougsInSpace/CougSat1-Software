#include <mbed.h>
// #include <iostream>
#include <CISConsole.h>

#include <Synth/ADF4360.h>
#include "IQArray.h"

#include <Trig.h>

DigitalOut statusLED(LED1, 1);
DigitalOut amp5(PA_PD, 0);

DigitalOut regEn5(REG_EN_5, 0); // enable pin for 5V voltage regulator

DigitalOut pcMod3(PC_3V1_1, 0); // 3.1V rail for modulator
DigitalOut pcMod5(PC_5V0_1_N, 0); // 5V rail for modulator
DigitalOut pcDemod(PC_3V1_2, 0); // 3.1V regulator for demodulator

DigitalOut antennaSwRX(ANTENNA_SW, 0); // switch antenna between mod/demod 0 = modulation, 1 = demodulation

SPI     spi(SPI_MOSI, SPI_MISO, SPI_SCK); // set up SPI
ADF4360 rfClkMod(spi, RFCLK_CS_MOD_N, 7, 20000000); // set up modulator synthesizer or 20k Hz crystal oscilator
ADF4360 rfClkDemod(spi, RFCLK_CS_DEMOD_N, 7, 20000000); // set up demodulator synthesizer for 20k Hz crystal oscilator

PortOut    dacBus(PortE, 0xFFC8);
PortOut    dacClk(PortE, 0x0010);
DigitalOut dacSleep(MODDAC_SLEEP, 0);

DigitalIn  lockDetect(RFCLK_MUXOUT_MOD);

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  
  // SWO_Channel * swo = SWOSingleton::Instance()->get();
  // while(true) {
  //   LOG("str", "hellooo\r\n");
  // }

  regEn5 = 1;
  wait_us(10e3);
  pcMod3 = 1;
  pcDemod = 0;
  pcMod5 = 1;
  wait_us(10e3);
  // rfClkDemod.setFrequency(443150000 * 2);
  rfClkMod.setFrequency(443150000);
  LOG("main", "has anything changed");
  wait_us(10e3);

  dacSleep = 1;
  wait_us(10);
  dacSleep = 0;
  wait_us(10e3);
  // cout << "hello" << endl;

  short KK7MWC[26] = {1,3,1,0,     // K
                      1,3,1,0,     // K
                      1,1,3,3,3,0, // 7
                      1,1,0,       // M
                      3,1,1,0,     // W
                      1,3,1,3,0};  // C

  int index = 0;
  // unsigned int phase = 45;
  while (true) {
    // LOG("main", "Lock Detected: %d", lockDetect.read());
    statusLED = !statusLED;
    // Morse code identification
    for (int i = 0; i < 26; i++) {
      if (KK7MWC[i] == 0) {
        wait_us(390e3);
      } else {

        for (int j = 0; j < KK7MWC[i]*10000; j++) {
          uint16_t codeI = LUT_SINE_10b[(j + 0) % TRIG_LUT_SIZE];
          dacClk         = 0x00;
          dacBus         = (codeI << 6) | 0x8;
          dacClk         = 0x10;
          wait_us(13);

          uint16_t codeQ = LUT_SINE_10b[(j + TRIG_LUT_COSINE) % TRIG_LUT_SIZE];
          dacClk         = 0x00;
          dacBus         = (codeQ << 6) | 0x0;
          dacClk         = 0x10;
          wait_us(13);
        }
        wait_us(130e3);
      }
    }

    // Go cougs! transmission
    for (int i = 0; i < 138*360; ++i) { // 10e3 is .4 baud, 1e3 is around 11.9
      // uint16_t codeI = uint16_t((LUT_SINE[i % TRIG_LUT_SIZE] + 1) / 2 * 1023
      // + 0.5);

      // uint16_t codeQ = uint16_t((LUT_SINE[(i + TRIG_LUT_COSINE) %
      // TRIG_LUT_SIZE] + 1) / 2 * 1023 + 0.5);

      // Setup and hold times met due to 80MHz => 12.5ns > 3ns
      // uint16_t codeI = LUT_SINE_10b[(i + 0) % TRIG_LUT_SIZE]; // fast way to do sine. Max value of Look-Up Table is 1023, a 10 bit int
      // uint16_t codeI = LUT_SINE_10b[(IQArr[index] + 0) % TRIG_LUT_SIZE];
      uint16_t codeI = MSG_DAC_FM_I[index];
      dacClk         = 0x00;
      // The mask for PortOut dacBus is 1111111111001000. CodeI will be some 16 bit int representing sine. The last 6 bits of the mask are 0 with the exception of the one in the 4th position.
      // The line of code before compares some int codeI in the binary form xxxxxxxxxxxxxxx with an or operator. The mask takes the first 10 positions of that int and keeps them, throws out the
      // rest. The int is left shifted 6 to account for the first 6 places of the mask being 0 with the exception of the 4th, which is taken care of with the or 0x8 (the number 8, or 1000 in binary)
      dacBus         = (codeI << 6) | 0x8; // bit math, compare 8 in binary (1000) to whatever codeI << 6 is. starts as 16 bit, shifted over to 22(?) bit
      dacClk         = 0x10;
      wait_ns(9000);

      // uint16_t codeQ = LUT_SINE_10b[(i + TRIG_LUT_COSINE) % TRIG_LUT_SIZE];//fast wat to do cosine?
      // uint16_t codeQ = LUT_SINE_10b[(IQArr[index] + TRIG_LUT_COSINE) % TRIG_LUT_SIZE];
      uint16_t codeQ = MSG_DAC_FM_Q[index];
      dacClk         = 0x00;
      dacBus         = (codeQ << 6) | 0x0;
      dacClk         = 0x10;
      wait_ns(9000);
      // 1/(10us*360) = 277Hz
    }
    // index = (index + 1) % dataLen;
    index = (index + 1) % IQArrLen;
  }
  return MBED_SUCCESS;
}
