/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file main.cpp
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Starts the IHU software
 *
 * Initializes IHU object and starts the eventQueue
 */

#include <mbed.h>

#include <ADC/AD7291.h>
#include <ADC/AD7689.h>
#include <CISError.h>

SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK);

/**
 * Program start routine
 * @return error code
 */
int main(void) {
  AD7689 adc(spi, PA_10);
  ADC *  pADC = &adc;
  pADC->getReferenceVoltage();
  return ERROR_SUCCESS;
}
