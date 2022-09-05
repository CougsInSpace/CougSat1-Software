#include <mbed.h>
// #include <CISConsole.h>

#include <Synth/ADF4360.h>

DigitalOut statusLED(LED1, 1);
DigitalOut regEn5(REG_EN_5, 0);
DigitalOut pcLDO3V3(PC_LDO_3V3, 1);
DigitalOut pcMod(PC_MOD_700_LV, 0);
DigitalOut pcDemod(PC_DEMOD_LV, 0);
DigitalOut pc230(PC_MOD_230_LV, 0);
SPI        spiRFClk(RFCLK_MOSI, NC, RFCLK_SCK);
ADF4360    rfClkMod(spiRFClk, RFCLK_CS_MOD, 7, 20000000);
ADF4360    rfClkDemod(spiRFClk, RFCLK_CS_DEMOD, 7, 20000000);
ADF4360    rfClk230(spiRFClk, RFCLK_CS_230, 7, 20000000);

// pc =  power control comms.sch
// turn pin pd3 (might have changed in newer version) on and off to beep
/**
 * Program start routine
 * @return error code
 */ 
int main(void) {
  regEn5   = 1; // Turns on 5v regulator which is power supply for power amplifier
  pcLDO3V3 = 0; 
  pcMod    = 1;
  pcDemod  = 1;
  pc230    = 1;
  wait_ms(10);
  rfClkMod.setFrequency(435000000);
  rfClkDemod.setFrequency(700000000);
  rfClk230.setFrequency(1275000000);

  while (true) {
    statusLED = !statusLED;
    wait_ms(500);
  }
  return MBED_SUCCESS;
}
