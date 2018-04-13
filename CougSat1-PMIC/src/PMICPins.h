/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file PMICPins.h
 * @author Bradley Davis
 * @date 13 Apr 2018
 * @brief Contains all hardware pins as constants
 */

#ifndef SRC_PMICPINS_H_
#define SRC_PMICPINS_H_

#define I2C_LOCAL_SDA PB_11
#define I2C_LOCAL_SCL PB_10

#define I2C_BUS_SDA PB_7
#define I2C_BUS_SCL PB_6

#define I2C_PMIC_SDA PC_1
#define I2C_PMIC_SCL PC_0

#endif /* SRC_PMICPINS_H_ */
