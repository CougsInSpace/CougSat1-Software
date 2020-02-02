#include "ADCSObjects.h"

I2CSlave i2cBus(BUS_I2C0_SDA, BUS_I2C0_SCL);

CDH cdh(i2cBus);