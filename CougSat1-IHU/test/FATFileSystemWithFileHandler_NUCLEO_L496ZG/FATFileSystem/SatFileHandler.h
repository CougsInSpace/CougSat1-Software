#include "mbed.h"
#include <cstdio>
//#include "SDBlockDevice.h"
#include "HeapBlockDevice.h"
#include <FATFileSystem.h>
#include <string>
#include <fstream>

//using namespace std;
using std::string;

// class that will handle all file read/write operations to a given medium on the sattelite
// inherits from SDBlockDevice as I'm adding functionality to the class. 
// though this does make the file handler limited to only an sd card over spi.
class SatFileHandler : public HeapBlockDevice{
public:
// create a SatFileHandler where the debug state can be enabled or disabled

SatFileHandler(HeapBlockDevice &sd, bool debug = false):HeapBlockDevice(sd){
    this->current = 0;
    this->priority = 0;
    
    if(debug){
        this->pc = new Serial(USBTX, USBRX);
    }

    // note all functions return 0 on succes
    // check the initilization status of the block device.
    int status = this->init();
    if (!status){
        if(pc){
            switch (status){
                case 1: pc->printf("device driver transaction failiure\n");
                break;
                case 2: pc->printf("SD missing or not plugged in\n");
                break;
                case  3: pc->printf("unusable card\n");
                break;
                case 4: pc->printf("crc error\n");
                break;
            }
        }
    }
    
    this->mFs = new FATFileSystem("fs");
    // check if the device is in the right format (FAT)
    status = this->mFs->mount(this->get_default_instance());

    if (!status){
        // check if the device reformatted properly if no give a critical error
        status = this->mFs->reformat(this->get_default_instance());
        if(pc && !status){
            pc->printf("CRITICAL ERROR\nFAILED MOUNT\nFAILED reformat");
        }
    }
};

SatFileHandler(PinName mosi, PinName miso, PinName sclk, PinName cs, uint64_t hz= 1000000, bool crc_on = 0, bool debug = false):HeapBlockDevice(2048, 512){
    this->current = 0;
    this->priority = 0;

    if(debug){
        this->pc = new Serial(USBTX, USBRX);
    }

    //make sure theres no error with the block device
    int status = this->init();
    if (!status){
        if(pc){
            switch (status){
                case 1: pc->printf("device driver transaction failiure\n");
                break;
                case 2: pc->printf("SD missing or not plugged in\n");
                break;
                case  3: pc->printf("unusable card\n");
                break;
                case 4: pc->printf("crc error\n");
                break;
            }
        }
    }

    this->mFs = new FATFileSystem("fs");
    // check if the device is in the right format (FAT)
    status = this->mFs->mount(this->get_default_instance());

    if (!status){
        // check if the device reformatted properly if no give a critical error
        status = this->mFs->reformat(this->get_default_instance());
        if(pc && !status){
            pc->printf("CRITICAL ERROR\nFAILED MOUNT\nFAILED reformat");
        }
    }
};

~SatFileHandler();

bool writef(string filenameBase, const char *message);
bool write(string filenameBase, string &message);

std::ofstream &read(string &fileNameFull);


private:

FATFileSystem *mFs;

// holds the current day of the sattelite will increment after each day
uint16_t current;

// hold the day that takes priority in removal if the device is running low on storage
uint16_t priority;

// debug purpose meant for testing purposes
Serial *pc;

bool status;

bool clean();

// checks for free memory
bool check();

};
