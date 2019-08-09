#ifndef _LIBRARY_DRIVER_ARDUCAM_ARDUCAM_H_
#define _LIBRARY_DRIVER_ARDUCAM_ARDUCAM_H_

#include <mbed.h>

// #define ARDUCHIP_TIM 0x03     // Timming control
// #define VSYNC_LEVEL_MASK 0x02 // 0 = High active ,     1 = Low active

// #define ARDUCHIP_TRIG 0x41 // Trigger source
// #define CAP_DONE_MASK 0x08

// #define ARDUCHIP_FIFO 0x04 // FIFO and I2C control
// #define FIFO_CLEAR_MASK 0x01
// #define FIFO_START_MASK 0x02
// #define FIFO_RDPTR_RST_MASK 0x10
// #define FIFO_WRPTR_RST_MASK 0x20

// #define BURST_FIFO_READ 0x3C  // Burst FIFO read operation
// #define SINGLE_FIFO_READ 0x3D // Single FIFO read operation

// #define FIFO_SIZE0 0x42 // Camera write FIFO size[7:0]
// #define FIFO_SIZE1 0x43 // Camera write FIFO size[15:8]
// #define FIFO_SIZE2 0x44 // Camera write FIFO size[18:16]

// #define ARDUCAM_VER_ADDR 0x40
// #define ARDUCHIP_TEST1 0x00 // TEST register
// #define ARDUCHIP_TEST_VAL 0x55

// #define ARDUCAM_2592x1944 0

class ArduCAM {
public:
  ArduCAM(SPI & spi, I2C & i2c, PinName pinSelect);

  // mbed_error_status_t initCam();
  // void                startCapture();
  // void                flushFifo();
  // mbed_error_status_t readFifo();
  // mbed_error_status_t readFifo(uint8_t * buf, uint8_t burstLength);
  // uint32_t            readFifoLength();
  // mbed_error_status_t setResolution(uint8_t size);
  // mbed_error_status_t setFormat(uint8_t format);
  // mbed_error_status_t getBit(uint8_t addr, uint8_t bit);

private:
  // mbed_error_status_t setFifoBurst(uint8_t);
  // mbed_error_status_t readReg(uint16_t addr, uint8_t * data);
  // mbed_error_status_t readReg16(uint16_t addr, uint16_t * data);
  // mbed_error_status_t writeReg(uint16_t addr, uint8_t data);
  // mbed_error_status_t writeReg16(uint16_t addr, uint16_t data);
  // mbed_error_status_t writeRegList(const struct CameraReg * regList);
  // void                setBit(uint8_t addr, uint8_t bit);
  // void                clearBit(uint8_t addr, uint8_t bit);
  // mbed_error_status_t readArduCAM(uint8_t addr);
  // void                writeArduCAM(uint8_t addr, uint8_t data);

  SPI &      spi;
  I2C &      i2c;
  DigitalOut select;
};

#endif /* _LIBRARY_DRIVER_ARDUCAM_ARDUCAM_H_ */
