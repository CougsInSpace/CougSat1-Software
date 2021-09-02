#include "ArduCAM.h"

// #include <CISConsole.h>

/**
 * Class to interface with an ArduCAM Module
 * @param spi connected to ArduChip
 * @param i2c connected to camera sensor
 * @param pinSelect to enable SPI with ArduChip
 */
ArduCAM::ArduCAM(SPI & spi, I2C & i2c, PinName pinSelect) :
  spi(spi), i2c(i2c), select(pinSelect, 1) {}

// /**
//  * Initializes camera to default settings
//  * Tests I2C and SPI communication
//  * @return error code
//  */
// mbed_error_status_t ArduCAM::initCam() {
//   mbed_error_status_t result;
//   uint16_t data;

//   result = readReg16(CAMERA_ID_ADDR, &data);
//   if (result != ERROR_SUCCESS) {
//     LOGD("ArduCAM", "Cannot I2C read from camera");
//     return result;
//   }

//   if (data != CAMERA_ID) {
//     LOGD("ArduCAM", "Camera id wrong: 0x%04x", data);
//     return ERROR_INVALID_DATA;
//   }

//   writeArduCAM(ARDUCHIP_TEST1, ARDUCHIP_TEST_VAL);
//   data = readArduCAM(ARDUCHIP_TEST1);
//   if (data != ARDUCHIP_TEST_VAL) {
//     LOGD("ArduCAM", "ArduChip test value failed: 0x%02x", data);
//     return ERROR_INVALID_DATA;
//   }

//   data = readArduCAM(ARDUCAM_VER_ADDR);
//   LOGD("ArduCAM", "ArduChip Version %d.%d", data >> 4, data & 0xF);

//   //Initialize settings
//   result = writeReg(0x3008, 0x80);
//   result |= writeRegList(OV5642_1280x960_RAW);
//   result |= writeRegList(OV5642_640x480_RAW);
//   if (result != ERROR_SUCCESS) {
//     LOGD("ArduCAM", "Writing sensor registers failed");
//     return result;
//   }

//   setBit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);

//   LOGD("ArduCAM", "Initialization complete");
//   return ERROR_SUCCESS;
// }

// /**
//  * Requests a frame to be taken on the ArduCAM
//  */
// void ArduCAM::startCapture() {
//   writeArduCAM(ARDUCHIP_FIFO, FIFO_START_MASK);
// }

// /**
//  * Flushes the frame buffer on the ArduCAM
//  */
// void ArduCAM::flushFifo() {
//   writeArduCAM(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
// }

// /**
//  * Reads the next byte from the frame buffer on the ArduCAM
//  * @return next byte in buffer
//  */
// mbed_error_status_t ArduCAM::readFifo() {
//   return readArduCAM(SINGLE_FIFO_READ);
// }

// /**
//  * Reads the next burst of bytes from the frame buffer on the ArduCAM
//  * @param buf to hold the burst
//  * @param burstLength to read from the buffer
//  * @return error code
//  */
// mbed_error_status_t ArduCAM::readFifo(uint8_t *buf, uint8_t burstLength) {
//   spi.lock();
//   select = 0;

//   spi.write(BURST_FIFO_READ & 0x7F);
//   while (burstLength > 0) {
//     *buf = spi.write(0x00);
//     buf++;
//     burstLength--;
//   }

//   select = 1;
//   spi.unlock();
//   return ERROR_SUCCESS;
// }

// /**
//  * Reads the number of bytes currently in the frame buffer on the ArduCAM
//  * @return length of frame buffer
//  */
// uint32_t ArduCAM::readFifoLength() {
//   uint32_t temp = readArduCAM(FIFO_SIZE2) << 16;
//   temp = temp | (readArduCAM(FIFO_SIZE1) << 8);
//   temp = temp | readArduCAM(FIFO_SIZE0);
//   return temp;
// }

// /**
//  * Changes the resolution of the camera
//  * @param size of the new resolution, see constants
//  * @return error code
//  */
// mbed_error_status_t ArduCAM::setResolution(uint8_t size) {
//   mbed_error_status_t result = writeRegList(OV5642_1280x960_RAW);
//   result |= writeRegList(OV5642_640x480_RAW);
// //  result |= writeReg(0x503d, 0x80);//Test pattern: Color bar
// //  result |= writeReg(0x503e, 0x00);
// //  result |= writeReg(0x503d, 0x85);//Test pattern: Color square
// //  result |= writeReg(0x503e, 0x12);
// //  result |= writeReg(0x503d, 0x85);//Test pattern: BW square
// //  result |= writeReg(0x503e, 0x1a);
// //  result |= writeReg(0x4741, 0x4);//Test pattern: DLI
//   if (result) {
//     LOGD("ArduCAM", "Writing sensor registers failed");
//     return result;
//   }
//   return ERROR_SUCCESS;
// }

// /**
//  * Changes the format of the image (Raw, jpeg, etc.)
//  * @param format, see constants
//  * @return error code
//  */
// mbed_error_status_t ArduCAM::setFormat(uint8_t format) {
//   return ERROR_NOT_SUPPORTED;
// }

// /**
//  * Reads a 8bit value from the camera's registers
//  * @param addr of register
//  * @param data to store register value
//  * @return error code
//  */
// mbed_error_status_t ArduCAM::readReg(uint16_t addr, uint8_t *data) {
//   uint8_t result;
//   char buf[2] =
//   { (char) (addr >> 8), (char) (addr & 0xFF) };
//   result = i2c.write(CAMERA_I2C_ADDR, buf, 2);
//   if (result != ERROR_SUCCESS) {
//     LOGD("ArduCAM", "Error commanding reg 0x%x", addr);
//     return ERROR_NACK;
//   }

//   result = i2c.read(CAMERA_I2C_ADDR, buf, 1);
//   if (result != ERROR_SUCCESS) {
//     LOGD("ArduCAM", "Error requesting reg 0x%x", addr);
//     return ERROR_NACK;
//   }

//   (*data) = buf[0];

//   return ERROR_SUCCESS;
// }

// /**
//  * Reads a 16bit value from the camera's registers
//  * @param addr of the first register
//  * @param data to store register value
//  * @return error code
//  */
// mbed_error_status_t ArduCAM::readReg16(uint16_t addr, uint16_t *data) {
//   uint8_t buf[2] =
//   { 0, 0 };
//   if (readReg(addr, &buf[0])) {
//     LOGD("ArduCAM", "Error reading reg 0x%x", addr);
//     return ERROR_READ;
//   }
//   if (readReg(addr + 1, &buf[1])) {
//     LOGD("ArduCAM", "Error reading reg 0x%x", addr + 1);
//     return ERROR_READ;
//   }

//   (*data) = (buf[0] << 8) | (buf[1]);
//   return ERROR_SUCCESS;
// }

// /**
//  * Writes a 8bit value to the camera's registers
//  * @param addr of the register
//  * @param data of the register
//  * @return error code
//  */
// mbed_error_status_t ArduCAM::writeReg(uint16_t addr, uint8_t data) {
//   uint8_t result;
//   char buf[3] =
//   { (char) (addr >> 8), (char) (addr & 0xFF), (char) data };
//   result = i2c.write(CAMERA_I2C_ADDR, buf, 3);
//   if (result != ERROR_SUCCESS) {
//     LOGD("ArduCAM", "Error writing reg 0x%x", addr);
//     return ERROR_NACK;
//   }

//   return ERROR_SUCCESS;
// }

// /**
//  * Writes a 16bit value to the camera's registers
//  * @param addr of the register
//  * @param data of the register
//  * @return error code
//  */
// mbed_error_status_t ArduCAM::writeReg16(uint16_t addr, uint16_t data) {
//   if (writeReg(addr, (data >> 8))) {
//     LOGD("ArduCAM", "Error writing reg 0x%x", addr);
//     return ERROR_WRITE;
//   }
//   if (writeReg(addr + 1, (data & 0xFF))) {
//     LOGD("ArduCAM", "Error writing reg 0x%x", addr + 1);
//     return ERROR_WRITE;
//   }
//   return ERROR_SUCCESS;
// }

// /**
//  * Writes a list of registers to camera
//  * @param regList of registers to write
//  * @return error code
//  */
// uint8_t ArduCAM::writeRegList(const struct CameraReg *regList) {
//   while (regList->regAddr != REGISTER_END_ADDR) {
//     if (writeReg(regList->regAddr, regList->regVal) != ERROR_SUCCESS) {
//       LOGD("ArduCAM", "Error writing reg 0x%x", regList->regAddr);
//       return ERROR_WRITE;
//     }
//     regList++;
//     wait_ms(2);
//   }
//   LOGD("ArduCAM", "Register list loaded");

//   return ERROR_SUCCESS;
// }

// /**
//  * Turns a single bit to (1) on the ArduChip
//  * @param addr of register
//  * @param bit to change
//  */
// void ArduCAM::setBit(uint8_t addr, uint8_t bit) {
//   uint8_t temp = readArduCAM(addr);
//   writeArduCAM(addr, temp | bit);
// }

// /**
//  * Turns a single bit to (0) on the ArduChip
//  * @param addr of register
//  * @param bit to change
//  */
// void ArduCAM::clearBit(uint8_t addr, uint8_t bit) {
//   uint8_t temp;
//   temp = readArduCAM(addr);
//   writeArduCAM(addr, temp & (~bit));
// }

// /**
//  * Reads a single bit from the ArduChip
//  * @param addr of register
//  * @param bit to read
//  * @return value of bit
//  */
// uint8_t ArduCAM::getBit(uint8_t addr, uint8_t bit) {
//   return readArduCAM(addr) & bit;
// }

// /**
//  * Read a register from the ArduChip
//  * @param addr of register
//  * @return value of register
//  */
// uint8_t ArduCAM::readArduCAM(uint8_t addr) {
//   spi.lock();
//   select = 0;

//   spi.write(addr & 0x7F);
//   uint8_t data = spi.write(0x00);

//   select = 1;
//   spi.unlock();
//   return data;
// }

// /**
//  * Writes a register on the ArduChip
//  * @param addr of register
//  * @param data to write
//  */
// void ArduCAM::writeArduCAM(uint8_t addr, uint8_t data) {
//   spi.lock();
//   select = 0;

//   spi.write(addr | 0x80);
//   spi.write(data);

//   select = 1;
//   spi.unlock();
// }
