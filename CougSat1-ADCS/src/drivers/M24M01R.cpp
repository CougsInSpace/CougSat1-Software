/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file M24M01R.cpp
 * @author Eric Curtland
 * @brief 
 * @date 2019-03-24
 * 
 */
#include "M24M01R.h"


M24M01::M24M01(I2C &i2c, PinName WC, PinName E1, PinName E2, uint8_t address) : chipEnable1(E1), chipEnable2(E2), writeControl(WC)
{
    uint8_t e1Address = address & 0x01, e2Address = address & 0x02;

    devAddr = 0;
    devAddr |= e1Address;
    devAddr |= e2Address;
    E1.write(e1Address);
    E2.write(e2Address);
    this->i2c = i2c;

    writeControl.write(1);
}
uint8_t M24M01::write(unit32_t address, unit8_t data)
{
    unit8_t deviceSelectCode = (DEVICE_TYPE_ID << 4) | ((address & 0x10000) >> 15) | (devAddr << 2) | MODE_WRITE, error = ERROR_SUCCESS;

    writeControl.write(0); // enable writing

    if (!i2c.write(devideSelectCode)) //send device select code to i2c bus
    {
        if (!i2c.write((address & 0xFF00) >> 8))//send first half of address
        {
            if (!i2c.write(address & 0xFF))//send second half of address
            {
                if (!i2c.write(data))//write byte to eeprom
                {
                }else{
                    error = ERROR_NACK;
                }
            }else{
                error = ERROR_OPEN_FAILED;
            }
        }else{
            error = ERROR_OPEN_FAILED;
        }
    }else{
        error = ERROR_OPEN_FAILED;
    }

    writeControl.write(1); // diable writing

    return error;
}
/**
 * @brief writes upto 256 bytes to the eeprom
 * 
 * @param address address of the first byte to write to
 * @param data pointer to char array which has data to be writen to eeprom
 * @param dataSize size of char array
 * @return uint8_t 
 */
uint8_t M24M01::write(unit32_t address, char* data, unit8_t dataSize){

 unit8_t deviceSelectCode = (DEVICE_TYPE_ID << 4) | ((address & 0x10000) >> 15) | (devAddr << 2) | MODE_WRITE, error = ERROR_SUCCESS;

    writeControl.write(0); // enable writing

    if (!i2c.write(devideSelectCode) && (!i2c.write((address & 0xFF00) >> 8 && (!i2c.write(address & 0xFF)) && //send device select code to i2c bus
    {
        for(unit8_t i = 0; i <dataSize && !error; ++i){
            ++address;
            if(address % 256 == 0){
                if(!i2c.write(deviceSelectCode)){
                    error = ERROR_OPEN_FAILED;
                }
            }
            i2c.write(*(data+i));
        }
    }else{
        error = ERROR_OPEN_FAILED;
    }

    writeControl.write(1); // diable writing

    return error;
}
/**
 * @brief reads 1 byte at provided address from eeprom
 * 
 * @param address 17 bit address for location of data
 * @param data pointer for indirect return of data
 */
uint8_t M24M01::read(unit32_t address, unit8_t *data)
{
    unit8_t deviceSelectCode = (DEVICE_TYPE_ID << 4) | ((address & 0x10000) >> 15) | (devAddr << 2) | MODE_WRITE, error = ERROR_SUCCESS;

    if (!i2c.write(devideSelectCode) && !i2c.write((address & 0xFF00) >> 8) && !i2c.write(address & 0xFF)) //send device select code to i2c bus
    {
        *data = i2c.read();
        
    }else{
        error = ERROR_OPEN_FAILED;
    }

    return error;
}
/**
 * @brief reads up to 256 byte from the eeprom
 * 
 * @param address start address
 * @param data character array for retuning the data read
 * @param dataSize amout of bytes to be read from
 * @return uint8_t error codes
 */
 uint8_t M24M01::read(unit32_t address, char* data, unit8_t dataSize){

    unit8_t deviceSelectCode = (DEVICE_TYPE_ID << 4) | ((address & 0x10000) >> 15) | (devAddr << 2) | MODE_WRITE, error = ERROR_SUCCESS;

    if (!i2c.write(devideSelectCode) &&!i2c.write((address & 0xFF00) >> 8) && !i2c.write(address & 0xFF)) //send device select code to i2c bus
    {
        for(unit8_t i = 0; i < dataSize -1; ++i){
            ++address;
            if(address % 256 == 0){
                if(!i2c.write(deviceSelectCode)){
                    error = ERROR_OPEN_FAILED;
                }
            }
            *(data +i) = i2c.read(1);

        }
            *(data + i) = i2c.read(); // read without ack to stop read operation
    }else{
        error = ERROR_OPEN_FAILED;
    }

    return error;
 }