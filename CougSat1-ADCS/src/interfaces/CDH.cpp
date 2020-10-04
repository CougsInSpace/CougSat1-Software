#include "CDH.h"

/**
 * @brief Construct a new CDH::CDH object
 *
 * @param i2c connected to the C&DH
 */
CDH::CDH(uint8_t addr, PinName sda, PinName scl) : i2c(sda, scl) {
  i2c.address(addr);
}

/**
 * @brief Checks if a message is available to read
 *
 * @return true is the PMIC is addressed
 */
bool CDH::messageReceived() {
  int state = i2c.receive();
  return state == mbed::I2CSlave::WriteAddressed;
}

/**
 * @brief Checks if a message is requested by master
 *
 * @return true is the PMIC is addressed
 */
bool CDH::messageRequested() {
  int state = i2c.receive();
  return state == mbed::I2CSlave::ReadAddressed;
}

/**
 * @brief Reads message from I2CSlave buffer into var message
 *
 */
void CDH::readCDH() {
  printf("Reading i2c buffer\n Messsage = ");
  i2c.read(message, MESSAGELENGTH);
  for (uint8_t i = 0; i < 9; i++) {
    printf("%c,", message[i]);
  }
  printf("\n");
}

/**
 * @brief Getter for private message array
 *
 */
char * CDH::getMessage() {
  return message;
}

void CDH::setReply(char * input) {
  strncpy(reply, input, 9);
}

void CDH::writeCDH() {
  printf("Writing i2c buffer\n Messsage = ");
  i2c.write(reply, MESSAGELENGTH);
  for (uint8_t i = 0; i < 9; i++) {
    printf("%c,", reply[i]);
  }
  printf("\n");
}