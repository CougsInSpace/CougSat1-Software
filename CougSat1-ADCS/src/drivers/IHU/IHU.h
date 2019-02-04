#include <mbed.h>
#include <rtos.h>

class IHUCOM
{
    private:
        char message[4];
    public:
        IHUCOM();
        void readIHU();
        char getMessage();
};