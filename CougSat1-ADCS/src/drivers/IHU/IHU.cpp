#include "IHU.h"
I2CSlave IHU(D14, D15);

IHUCOM::IHUCOM()
{
    IHU.address(0x99);
}

void IHUCOM::readIHU(){    
    printf("Listening for I2C\r\n");
    int test = 0;
    while(test != 3){
        test = IHU.receive();
    }
    printf("test:%D\r\n", test); 
    IHU.read(message, 4);
    int i = 0;
    char c = message[i];
    while(c != NULL){
        c = message[i];
        printf("%c\n", c);
        i++;
    }
}