#include <mbed.h>

DigitalOut statusLED(LED1);

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  while (true) {
    statusLED = !statusLED;
    wait_ms(500);
  }
  return MBED_SUCCESS;
}
