/*
 * CameraSensor.h
 *
 *  Created on: Nov 6, 2017
 *      Author: Bradley
 */

#ifndef SRC_DRIVERS_CONFIGURATIONS_CAMERASENSOR_H_
#define SRC_DRIVERS_CONFIGURATIONS_CAMERASENSOR_H_

#define REGISTER_END_ADDR 0xFFFF

struct CameraReg {
	uint16_t regAddr;
	uint8_t regVal;
};

#endif /* SRC_DRIVERS_CONFIGURATIONS_CAMERASENSOR_H_ */
