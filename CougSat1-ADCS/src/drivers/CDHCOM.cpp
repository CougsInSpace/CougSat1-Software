/******************************************************************************
 * Copyright (c) 2019 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file CDHCOM.cpp
 * @author Ryal O'Neil
 * @date 2019-1-27
 * @brief Communicates between ADCS and CDH
 *
 * Initializes CDHCOM object for communication with CDH
 */

#include "CDHCOM.h"
#define MESSAGEREADY 3
#define MESSAGELENGTH 4

/**
 * @brief Construct a new CDHCOM::CDHCOM object
 * 
 * @param addr 
 * @param sda 
 * @param scl 
 */
CDHCOM::CDHCOM(int addr, PinName sda, PinName scl) : CDH(sda, scl)
{
    CDH.address(addr);
}

/**
 * @brief Checks if there is a message waiting. Times out after aproximatly 900ms
 * 
 * @return true 
 * @return false 
 */
bool CDHCOM::checkRead()
{
    if (CDH.receive() == MESSAGEREADY)
        return true;
    else
        return false;
}

/**
 * @brief Reads message from I2CSlave buffer into var message
 * 
 */
void CDHCOM::readCDH()
{
    CDH.read(message, MESSAGELENGTH);
    for (int i = 0; message[i] != NULL; i++)
    {
        printf("%c\n", message[i]);
    }
}