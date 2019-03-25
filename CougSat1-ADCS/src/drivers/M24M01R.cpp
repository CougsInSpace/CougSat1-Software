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
    unit8_t deviceSelectCode = (DEVICE_TYPE_ID << 4) | ((address & 0x10000) >> 15) | (devAddr << 2) | MODE_WRITE, error = 0;

    writeControl.write(0); // enable writing

    if (!i2c.write(devideSelectCode))
    {
        if (!i2c.write((address & 0xFF00) >> 8))
        {
            if (!i2c.write(address & 0xFF))
            {
                if (!i2c.write(data))
                {
                }else{
                    error = 3;
                }
            }else{
                error = 2;
            }
        }else{
            error = 2;
        }
    }else{
        error = 1;
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
}