/******************************************************************************
 * Copyright (c) 2018 by Cougs in Space - Washington State University         *
 * Cougs in Space website: cis.vcea.wsu.edu                                   *
 *                                                                            *
 * This file is a part of flight and/or ground software for Cougs in Space's  *
 * satellites. This file is proprietary and confidential.                     *
 * Unauthorized copying of this file, via any medium is strictly prohibited.  *
 ******************************************************************************/
/**
 * @file IMU.cpp
 * @author Eric Curtland
 * @date 21 OCTOBER 18
 * @brief Provides an interface to the on-board IMU 
 *
*/

#include "IMU.h"

// TODO: implement
IMU::IMU(imu_operating_mode mode){
	_mode = mode;
}

// TODO: implement
IMU::~IMU(){
	//free up any dynamic memory
}

// TODO: implement
IMU::getTemp(void){
	//reads temperature from data registar and returns it;
}

// TODO: implement
IMU::setMode(imu_operating_mode mode){
	//sets imu's operating mode
}

// TODO: implement
IMU::setPowerMode(imu_power_mode mode){
	//sets the imu's to normal, low ,or suspended
}

// TODO: implement
IMU::getSensorOffsets(imu_offsets &offsets){
	//reads the current sensor offsets into a offset struct
}

// TODO: implement
IMU::setSensorOffsets(imu_offsets &offsets){
	//uses a offset struct to set the sensor offsets
}

// TODO: implement
IMU::read(sensor_data &gyro_data, sensor_data &mag_data, sensor_data &accel_data, quat_data &quaternian){
	//reads the data from the registers and puts it in the the apropriet struct;
}

// TODO: implement
IMU::read8(imu_register_address reg){
	//reads a byte from the data regiter useing the address of the first bit
}

// TODO: implement
IMU::write8(imu_register_address reg, byte value){
	//writes a byte to the data registers using the address of the first bit
}