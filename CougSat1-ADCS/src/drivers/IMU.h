/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file IMU.h
 * @author Eric Curtland
 * @date 21 OCTOBER 18
 * @brief Provides an interface to the on-board IMU 
 *
*/

#ifndef SRC_SYSTEMINTERFACES_IMU_H_
#define SRC_SYSTEMINTERFACES_IMU_H_

#include <mbed.h>


class IMU{
	public:
	IMU(imu_operating_mode mode);
	~IMU();
	
	typedef enum {
	
	}imu_operating_mode;

	typedef enum {
	
	}imu_power_mode;

	typedef enum {
	
	}imu_register_address;
	
	int8_t getTemp(void);
	uint8_t   setMode(imu_operating_mode mode);
	uint8_t   setPowerMode(imu_power_mode mode);
	uint8_t   getSensorOffsets(imu_offsets &offsets);
	uint8_t   setSensorOffsets(imu_offsets &offsets);
	uint8_t   read();
	
	
	
	private:
	
	byte read8(imu_register_address reg);
	void write8(imu_register_address reg, byte value);
	
	imu_operating_mode _mode; // current opperating mode of the imu
	uint8_t            _address;//address of the imu
	uint16_t           _accelX;
	uint16_t           _accelY;
	uint16_t           _accelZ;
	uint16_t           _gyroX;
	uint16_t           _gyroY;
	uint16_t           _gyroZ;
	uint16_t           _magX;
	uint16_t           _magY;
	uint16_t           _magZ;
	uint16_t           _quatX;
	uint16_t           _quatY;
	uint16_t           _quatZ;
	uint16_t           _quatW;
	
}

#endif /* !SRC_SYSTEMINTERFACES_IMU_H_*/