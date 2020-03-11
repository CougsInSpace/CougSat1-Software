#ifndef _SRC_INU_OBJECTS_H_
#define _SRC_INU_OBJECTS_H_

#include "Configuration.h"

#include <Thermistor.h>
#include <mbed.h>

extern I2C i2c0;
extern I2C i2c1;

extern SPI spi0;
extern SPI spi1;
extern SPI spi2;

extern DigitalIn i2cIRQ0;
extern DigitalIn i2cIRQ1;

extern DigitalIn sdSW0;
extern DigitalIn sdSW1;
extern DigitalOut sdCS0;
extern DigitalOut sdCS1;

extern DigitalOut gpio0;
extern DigitalOut gpio1;
extern DigitalOut gpio2;
extern DigitalOut gpio3;
extern DigitalOut gpio4;
extern DigitalOut gpio5;

extern DigitalOut statusLED;

extern DigitalOut ctrlSync;
extern DigitalOut ctrlResetN;

extern EventQueue queue;

#endif /* _SRC_INU_OBJECTS_H_ */