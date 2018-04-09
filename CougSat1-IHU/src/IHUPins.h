/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file IHUPins.h
 * @author Bradley Davis
 * @date 6 Mar 2018
 * @brief Contains all hardware pins as constants
 */

#ifndef SRC_IHU_H_
#define SRC_IHU_H_

#define UART0_TX PD_8
#define UART0_RX PD_9

#define SPI_MOSI PA_7
#define SPI_MISO PA_6
#define SPI_SCLK PA_5

#define SPI_CS_CAM0 PD_14

#define I2C0_SDA  PB_9
#define I2C0_SCL  PB_8

#define I2C1_SDA  PF_0
#define I2C1_SCL  PF_1

#endif /* SRC_IHU_H_ */
