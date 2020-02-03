#include "CDH.h"

/**
 * @brief Construct a new CDH::CDH object
 *
 * @param i2cslave is given the proper address
 * @param i2cslave is given the sda pin
 * @param i2cslave is given the scl pin
 */
CDH::CDH(uint8_t addr, PinName sda, PinName scl) : i2c(sda, scl) 
{
  i2c.address(addr);
}

/**
 * @brief Checks if a message is available to read
 *
 * @return true if the i2c state is WriteAddressed
 */
bool CDH::messageReceived() {
  int state = i2c.receive();
  return state == mbed::I2CSlave::WriteAddressed ;
}

/**
 * @brief Checks if a message is requested by master
 *
 * @return true if the i2c state is ReadAddressed
 */
bool CDH::messageRequested() {
  int state = i2c.receive();
  return state == mbed::I2CSlave::ReadAddressed ;
}

/**
 * @brief Reads message from I2CSlave buffer into var message
 * 
 */
void CDH::readI2C()
{
  printf("Reading i2c buffer\n Messsage = ");
  if(i2c.read(message, MESSAGELENGTH) != 0)
  {
    //ERROR("ADCS", "Failed");
  }
  for (uint8_t i = 0; i < MESSAGELENGTH; i++)
  {
      printf("%c,", message[i]);
  }
  printf("\n");
}

/**
 * @brief Getter for private message array
 * 
 * @return message private message array
 */
char* CDH::getMessage()
{
  return message;
}

/**
 * @brief Writes from var message into I2CSlave buffer 
 * 
 */
void CDH::writeI2C()
{
  printf("Writing i2c buffer\n Messsage = ");
  if(i2c.write(message, MESSAGELENGTH) != 0)
  {
    //ERROR("ADCS", "Failed");
  }
  for (uint8_t i = 0; i < MESSAGELENGTH; i++)
  {
      printf("%c,", message[i]);
  }
  printf("\n");
}
