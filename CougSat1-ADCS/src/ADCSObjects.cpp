#include "ADCSObjects.h"

/**
 * @brief Function for cdh communication thread
 *
 */
void ADCS::cdhThread() {
  while (true) {
    // Get message from CDH
    if (cdh.messageReceived()) {
      cdh.readCDH();
    }
    // Parse the message
    char      message[9];
    char      reply[9];
    uint8_t * messageCode = (uint8_t *)message;
    strncpy(message, cdh.getMessage(), 9);
    printf("messageCode = %x\n", *messageCode);
    switch (message[0]) {
      case 0x00:
        // CDH requested our coordinates
        printf("LatLong requested\n");
        float * latitude  = (float *)(reply);
        float * longitude = (float *)(reply + sizeof(float));
        *latitude         = 46.730939;
        *longitude        = -117.167933;
        printf("reply = %d,%d,%d,%d,%d,%d,%d,%d,%d\n", reply[0], reply[1],
            reply[2], reply[3], reply[4], reply[5], reply[6], reply[7],
            reply[8]);
        cdh.setReply(reply);
        break;
        // case 0x01:
        // code
        break;
        // case 0x02:
        // code
        break;
        // case 0x03:
        // code
        break;
        // case 0x04:
        // code
        break;
        // case 0x05:
        // code
        break;
        // case 0x06:
        // code
        break;
        // case 0x07:
        // code
        break;
        // case 0x08:
        // code
        break;
        // case 0x09:
        // code
        break;
        // case 0x10:
        // code
        break;
        // case 0x11:
        // code
        break;
    }

    if (cdh.messageRequested()) {
      cdh.writeCDH();
    }
    ThisThread::sleep_for(PERIOD_MS_POLLING_SLEEP);
  }
}

/**
 * @brief Construct a new ADCS::ADCS object
 *
 */
#define TEST_IHU_ADDRESS 0xAC
#define BUS_I2C0_SDA PinName(25)
#define BUS_I2C0_SCL PinName(24)
ADCS::ADCS() : cdh(TEST_IHU_ADDRESS, BUS_I2C0_SDA, BUS_I2C0_SCL) {
  // monitor.set_priority(osPriorityNormal);
  cdhRead.start(callback(this, &ADCS::cdhThread));
  cdhRead.set_priority(osPriorityRealtime);
  // monitor.start(callback(this, &ADCS::monitorThread));
}

/**
 * @brief main function for ADCS object
 *
 */
void ADCS::initThread() {
  printf("Outside Main!\r\n");
  DigitalOut led1(PinName(5));
  while (true) {
    printf("Process Main!\r\n");
    led1 = !led1;
    ThisThread::sleep_for(1000);
    // ThisThread::flags_wait_all(0x00);
  }
}