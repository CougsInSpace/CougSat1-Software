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

typedef struct {
	
}imu_offsets;

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t z;
	uint16_t w;
}quat_data;

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t z;
}sensor_data;

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
	void   setMode(imu_operating_mode mode);
	void   setPowerMode(imu_power_mode mode);
	void   getSensorOffsets(imu_offsets &offsets);
	void   setSensorOffsets(imu_offsets &offsets);
	void   read(sensor_data &gyro_data, sensor_data &mag_data, sensor_data &accel_data, quat_data &quaternian);
	
	
	
	private:
	
	byte read8(imu_register_address reg);
	void write8(imu_register_address reg, byte value);
	
	imu_operating_mode _mode; // current opperating mode of the imu
	uint8_t            _address;//address of the imu
	
}

#endif /* !SRC_SYSTEMINTERFACES_IMU_H_*/