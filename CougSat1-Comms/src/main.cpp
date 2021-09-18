#include <mbed.h>
// #include <CISConsole.h>

#include <Synth/ADF4360.h>

#include <Trig.h>

DigitalOut statusLED(LED1, 1);

DigitalOut regEn5(REG_EN_5, 0);

DigitalOut pcMod3(PC_3V1_1, 0);
DigitalOut pcMod5(PC_5V0_1_N, 1);
DigitalOut pcDemod(PC_3V1_2, 0);

DigitalOut antennaSwRX(ANTENNA_SW, 0);

SPI     spi(SPI_MOSI, SPI_MISO, SPI_SCK);
ADF4360 rfClkMod(spi, RFCLK_CS_MOD_N, 7, 20000000);
ADF4360 rfClkDemod(spi, RFCLK_CS_DEMOD_N, 7, 20000000);

PortOut    dacBus(PortE, 0xFFC8);
PortOut    dacClk(PortE, 0x0010);
DigitalOut dacSleep(MODDAC_SLEEP, 0);

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  regEn5 = 1;
  wait_us(10e3);
  pcMod3 = 1;
  pcMod5 = 1;
  wait_us(10e3);
  rfClkMod.setFrequency(435000000);
  wait_us(10e3);

  dacSleep = 1;
  wait_us(10);
  dacSleep = 0;
  wait_us(10e3);

  while (true) {
    statusLED = !statusLED;
    for (int i = 0; i < 138 * 360; ++i) {
      // uint16_t codeI = uint16_t((LUT_SINE[i % TRIG_LUT_SIZE] + 1) / 2 * 1023
      // + 0.5);

      // uint16_t codeQ = uint16_t((LUT_SINE[(i + TRIG_LUT_COSINE) %
      // TRIG_LUT_SIZE] + 1) / 2 * 1023 + 0.5);

      // Setup and hold times met due to 80MHz => 12.5ns > 3ns
      uint16_t codeI = LUT_SINE_10b[(i + 0) % TRIG_LUT_SIZE];
      dacClk         = 0x00;
      dacBus         = (codeI << 6) | 0x8;
      dacClk         = 0x10;
      wait_us(5);

      uint16_t codeQ = LUT_SINE_10b[(i + TRIG_LUT_COSINE) % TRIG_LUT_SIZE];
      dacClk         = 0x00;
      dacBus         = (codeQ << 6) | 0x0;
      dacClk         = 0x10;
      wait_us(5);
      // 1/(10us*360) = 277Hz
    }
  }
  return MBED_SUCCESS;
}
