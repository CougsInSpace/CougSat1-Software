/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file M24M01R.h
 * @author Eric Curtland
 * @brief 
 * @date 2019-03-24
 * 
 */
#ifndef M24M01R_H_
#define M24M01R_H_

#define M24M01_PAGE_SIZE 256
#define DEVICE_TYPE_ID 0x10
#define MODE_WRITE 0
#define MODE_READ 1

#include <mbed.h>

typedef enum
{
    noError = 0;
    badDeviceAddress = 1;
    badMemoryAddress = 2;
    errorInWritig = 3;

}

class M24M01
{

  public:
    M24M01(I2C &i2c, PinName WC, PinName E1, PinName E2, uint8_t address);

    uint8_t write(unit32_t address, unit8_t data);
    uint8_t read(unit32_t address, unit8_t *data);

  private:
    unit8_t devAddr;
    I2c &_i2c;
    DigitalOut chipEnable1;
    DigitalOut chipEnable2;
    DigitalOut writeControl;
}

#endif //!M24M01R_H_