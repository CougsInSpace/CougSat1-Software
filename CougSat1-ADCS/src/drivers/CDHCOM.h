/******************************************************************************
 * Copyright (c) 2019 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file CDHCOM.h
 * @author Ryal O'Neil
 * @date 2019-1-27
 * @brief Communicates between ADCS and CDH
 *
 * Declares CDHCOM object for communication with CDH
 */

#include <mbed.h>
#include <rtos.h>

//IHU Communication

class CDHCOM
{
  private:
    I2CSlave CDH;
    char message[4];

  public:
    CDHCOM(int addr, PinName sda, PinName scl);
    void readCDH();
    char getMessage();
    bool checkRead();
};