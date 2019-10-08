#include "CDH.h"

#include <CISConsole.h>

/**
 * @brief Construct a new CDH::CDH object
 *
 * @param i2c connected to the C&DH
 */
CDH::CDH(uint8_t addr, PinName sda, PinName scl) : i2c(sda, scl) 
{
  i2c.address(addr);
}

/**
 * @brief Checks if a message is available to read
 *
 * @return true is the PMIC is addressed
 */
bool CDH::hasMessage() {
  int state = i2c.receive();
  return state == I2CSlave::ReadAddressed;
}

/**
 * @brief Reads message from I2CSlave buffer into var message
 * 
 */
void CDH::readCDH()
{
    i2c.read(message, MESSAGELENGTH);
    i2c.write(message, MESSAGELENGTH);
    // for (uint8_t i = 0; message[i] != NULL; i++)
    // {
    //     printf("%c\n", message[i]);
    // }
}

/**
 * @brief Getter for private message array
 * 
 */
char* CDH::getMessage()
{
  return message;
}
