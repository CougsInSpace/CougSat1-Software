#include <mbed.h>
#include <rtos.h>
#include "../../ADCSPins.h"

//IHU Communication
extern I2CSlave IHU;

class IHUCOM
{
    private:
        char message[4];
    public:
        IHUCOM();
        void readIHU();
        char getMessage();
};