#include <mbed.h>
// #include <iostream>
#include <CISConsole.h>

HAL_RCC_MCOConfig(PA_8, RCC_MC01SOURCE_SYSCLOCK, RCC_MCODIV_2);
// #include <Synth/ADF4360.h>

// #include <Trig.h>

// DigitalOut statusLED(LED1, 1);

// DigitalOut regEn5(REG_EN_5, 0); // goes to TPS61236P, steps up voltage while stepping down current

// DigitalOut pcMod3(PC_3V1_1, 0); // LP5907MFX-3.1, voltage regulator  
// DigitalOut pcMod5(PC_5V0_1_N, 1); // INA199C1, current shunt monitor
// DigitalOut pcDemod(PC_3V1_2, 0);

// DigitalOut antennaSwRX(ANTENNA_SW, 0); // goes directly to antenna

// SPI     spi(SPI_MOSI, SPI_MISO, SPI_SCK); // Serial Peripheral Interface, similiar to I2C
// ADF4360 rfClkMod(spi, RFCLK_CS_MOD_N, 7, 20000000); // signal send oscillator?
// ADF4360 rfClkDemod(spi, RFCLK_CS_DEMOD_N, 7, 20000000); // signal receive oscillator?

// PortOut    dacBus(PortE, 0xFFC8); // PortE enum to 4. 0xFFC8=1111111111001000 is a mask, decides which bits in the output will be used. Apparently used to control the signal. Is 32 bit?
// PortOut    dacClk(PortE, 0x0010); // 0x0010=00010000 just the clock pin?
// DigitalOut dacSleep(MODDAC_SLEEP, 0);

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  
  // SWO_Channel * swo = SWOSingleton::Instance()->get();
  while(true) {
    LOG("str", "hellooo\r\n");
  }

//   regEn5 = 1;
//   wait_us(10e3);
//   pcMod3 = 1;
//   pcMod5 = 1;
//   wait_us(10e3);
//   rfClkMod.setFrequency(435000000);
//   wait_us(10e3);

//   dacSleep = 1;
//   wait_us(10);
//   dacSleep = 0;
//   wait_us(10e3);
//   // cout << "hello" << endl;

//   while (true) {
//     // printf("helloo2");
//     statusLED = !statusLED;
//     for (int i = 0; i < 138 * 360; ++i) {
//       // uint16_t codeI = uint16_t((LUT_SINE[i % TRIG_LUT_SIZE] + 1) / 2 * 1023
//       // + 0.5);

//       // uint16_t codeQ = uint16_t((LUT_SINE[(i + TRIG_LUT_COSINE) %
//       // TRIG_LUT_SIZE] + 1) / 2 * 1023 + 0.5);

//       // Setup and hold times met due to 80MHz => 12.5ns > 3ns
//       uint16_t codeI = LUT_SINE_10b[(i + 0) % TRIG_LUT_SIZE]; // fast way to do sine. Max value of Look-Up Table is 752, a 10 bit int
//       dacClk         = 0x00;
//       // The mask for PortOut dacBus is 1111111111001000. CodeI will be some 16 bit int representing sine. The last 6 bits of the mask are 0 with the exception of the one in the 4th position.
//       // The line of code before compares some int codeI in the binary form xxxxxxxxxxxxxxx with an or operator. The mask takes the first 10 positions of that int and keeps them, throws out the
//       // rest. The int is left shifted 6 to account for the first 6 places of the mask being 0 with the exception of the 4th, which is taken care of with the or 0x8 (the number 8, or 1000 in binary)
//       dacBus         = (codeI << 6) | 0x8; // bit math, compare 8 in binary (1000) to whatever codeI << 6 is. starts as 16 bit, shifted over to 22(?) bit
//       dacClk         = 0x10;
//       wait_us(5);

//       uint16_t codeQ = LUT_SINE_10b[(i + TRIG_LUT_COSINE) % TRIG_LUT_SIZE];//fast wat to do cosine?
//       dacClk         = 0x00;
//       dacBus         = (codeQ << 6) | 0x0;
//       dacClk         = 0x10;
//       wait_us(5);
//       // 1/(10us*360) = 277Hz
//     }
//   }
//   return MBED_SUCCESS;
}
