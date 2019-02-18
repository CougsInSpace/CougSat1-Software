#include "IHUCOM.h"


IHUCOM::IHUCOM(): IHU(PIN_I2C_BUS_SDA, PIN_I2C_BUS_SCL)
{
    IHU.address(0x99);
}

bool IHUCOM::checkRead(){
    if(IHU.receive() == 3)
        return true;
    else
        return false;
}

void IHUCOM::readIHU(){    
    //printf("Listening for I2C\r\n");
    // int test = 0;
    // while(test != 3){
    //     test = IHU.receive();
    // }
    // printf("test:%D\r\n", test); 
    IHU.read(message, 4);
    int i = 0;
    char c = message[i];
    while(c != NULL){
        c = message[i];
        printf("%c\n", c);
        i++;
    }
}