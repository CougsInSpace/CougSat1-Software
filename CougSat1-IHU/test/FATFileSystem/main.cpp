/* October 13 2019
FATFileSystem mounted on SDBlockDevice written by James Ponce and
Christian Rouhana. This specifically works on a NUCLEO-L476RG board. 
There is a huge variability from board to board as many don't function properly.

This program will create and mount a FAT file system, write a text file, and 
then read said text file. It will report back whether or not each step of the process
was successful. */

#include "mbed.h"
#include <cstdio>
#include "SDBlockDevice.h"
#include <FATFileSystem.h>

SDBlockDevice sd(D11, D12, D13, D10);
FATFileSystem fs("sd");
Serial pc(SERIAL_TX, SERIAL_RX);

int main() {
    pc.printf("Start \n");
    int a = sd.init();
    //fs.format(&sd); //Uncomment if this is the first time running this
    fs.mount(&sd);
    pc.printf("%X \n", a);
    /* Checks for existing text file. This will be successful on a second run
       but not the first run. The file system must be mounted and the file must
       be written first */
    FILE *fp = fopen("/sd/SDBlockDeviceTest.txt", "r");
    if(fp){
        pc.printf("opened existing file\n");
        char buf [20] = {0};
        fgets(buf, 20, fp);
        pc.printf("%s\n", buf);
        fclose(fp);
        pc.printf("closing file now\n");
    } else {
        pc.printf("failed to open existing file\n");
    }
    
    // Writes new test file
    fp = fopen("/sd/SDBlockDeviceTest.txt", "w"); 
    if (fp) { // Check for creation success
        pc.printf("Creation Successful.\n");
        fprintf(fp, "hello world\n");
        pc.printf("finished writing now closing\n");
        fclose(fp);
        // Reads newly created file and checks for success
        fp = fopen("/sd/SDBlockDeviceTest.txt", "r");
        if(fp){
            pc.printf("opened for reading\n");
            char buf [20] = {0};
            fgets(buf, 20, fp);
            pc.printf("%s\n", buf);
            fclose(fp);
            pc.printf("closing file now\n");
        } else {
            pc.printf("ERROR FAILED TO OPEN");
        }
    } else {
        //pc.printf("Creation Fail.\n\r");
    }
    sd.deinit();
    pc.printf("End");
}
