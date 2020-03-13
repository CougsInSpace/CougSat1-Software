#include "IFJR.h"

IFJR &IFJR::getInstance()
{
        static IFJR ifjr;
        return ifjr;
}

IFJR::~IFJR()
{
        delete i2c;
}

mbed_error_status_t IFJR::initialize()
{
        return MBED_SUCCESS;
}

void IFJR::setI2C(I2C &i2c)
{
        IFJR::getInstance().i2c = &i2c;
}