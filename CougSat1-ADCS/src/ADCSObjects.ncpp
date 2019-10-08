#include "ADCSObjects.h"

/**
 * @brief Function for the polling thread
 * 
 */
void ADCS::monitorThread()
{
    while (true)
    {
        ThisThread::sleep_for(PERIOD_MS_POLLING_SLEEP);
        //printf("Checking for events!\r\n");
        if (cdh.hasMessage())
        {
            cdhRead.flags_set(CDH_FLAG_MSG_READY);
            //printf("Turning on I2C\r\n");
        }
    }
}

/**
 * @brief Function for cdh communication thread
 * 
 */
void ADCS::cdhThread()
{
    while (true)
    {
        ThisThread::flags_wait_all(CDH_FLAG_MSG_READY);
        cdh.readCDH();
    }
}

/**
 * @brief Construct a new ADCS::ADCS object
 * 
 */
#define TEST_IHU_ADDRESS 0xAC
ADCS::ADCS() : cdh(TEST_IHU_ADDRESS, BUS_I2C0_SDA, BUS_I2C0_SCL)
{
    monitor.set_priority(osPriorityRealtime);
    cdhRead.set_priority(osPriorityRealtime);
    cdhRead.start(callback(this, &ADCS::cdhThread));
    monitor.start(callback(this, &ADCS::monitorThread));
}

/**
 * @brief main function for ADCS object
 * 
 */
void ADCS::initThread()
{
    printf("Outside Main!\r\n");
    DigitalOut led1(LED1);
    while (true)
    {
        printf("Process Main!\r\n");
        led1 = !led1;
        ThisThread::sleep_for(1000);
    }
}

