#include <mbed.h>
// #include <CISConsole.h>

#include <Synth/ADF4360.h>

DigitalOut statusLED(PA_5, 1);
DigitalOut pcLDO3V3(PC_8, 1);
SPI        spiRFClk(PA_12, NC, PA_1);
ADF4360    rfClkMod(spiRFClk, PA_3, 7, 20000000);

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  pcLDO3V3 = 0;
  rfClkMod.setFrequency(435000000);

  while (true) {
    statusLED = !statusLED;
    wait_ms(500);
  }
  return MBED_SUCCESS;
}
