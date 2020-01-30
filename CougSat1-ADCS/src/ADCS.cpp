#include "ADCS.h"

/**
 * @brief Function for cdh communication thread
 * 
 */
void ADCS::cdhThread()
{
    while (true)
    {
    if(cdh.messageReceived())
    {
        cdh.readI2C();
    }
    if(cdh.messageRequested())
    {
        cdh.writeI2C();
    }
    ThisThread::sleep_for(PERIOD_MS_POLLING_SLEEP);
    }
}

/**
 * @brief Construct a new ADCS::ADCS object
 * 
 * @param cdh is given the proper address for the i2cslave
 * @param cdh is given the sda pin for the i2cslave
 * @param cdh is given the scl pin for the i2cslave
 */
#define TEST_IHU_ADDRESS 0xAC
#define BUS_I2C0_SDA PinName(25)
#define BUS_I2C0_SCL PinName(24)
ADCS::ADCS() : cdh(TEST_IHU_ADDRESS, BUS_I2C0_SDA, BUS_I2C0_SCL)
{
    //monitor.set_priority(osPriorityNormal);
    //cdhRead.set_priority(osPriorityNormal);
    cdhRead.start(callback(this, &ADCS::cdhThread));
    //monitor.start(callback(this, &ADCS::monitorThread));
}

/**
 * @brief main function for ADCS object
 * 
 */
void ADCS::startThread()
{
    printf("Outside Main!\r\n");
    DigitalOut led1(PinName(5));
    while (true)
    {
        printf("Process Main!\r\n");
        led1 = !led1;
        ThisThread::sleep_for(1000);
        //ThisThread::flags_wait_all(0x00);
    }
}