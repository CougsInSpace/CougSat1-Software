#include "T6713.h"

/**
 * @brief Construct a new T6713 object
 * 
 * @param i2c  The I2C with which the driver works
 * @param addr The slave address of the sensor on the i2c
 */
T6713::T6713(I2C &i2c, uint8_t addr)
{
    this->i2c = i2c;
    this->addr = addr;
}

/**
 * @brief Destroy the T6173 object.
 * As of now, the destructor does not need anything.
 * 
 */
~T6713::T6173()
{

}

/**
 * @brief Gets the PPM (CO2 level) reading of the sensor
 * 
 * @return int 
 */
int T6173::readPPM()
{
    char data[4];
    i2c.write(this->addr, READ_CO2_COMMAND, 5)/;
    thread_sleep_for(2000);
    i2c.read(this->addr, data, 4);
    return (((data[2] & 0x3F ) << 8) | data[3]);
}