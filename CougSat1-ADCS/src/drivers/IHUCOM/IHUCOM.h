#include <mbed.h>
#include <rtos.h>
#include "ADCSPins.h"

//IHU Communication

class IHUCOM
{
    private:
        I2CSlave IHU;
        char message[4];
    public:
        IHUCOM();
        void readIHU();
        char getMessage();
};