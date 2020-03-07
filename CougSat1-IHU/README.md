# Internal Housekeeping Unit

Current status:  
Working on the FileSystem.

## Installation and Compiling

This is a very complicated process. All the files to compile are located in the CougSat1-IHU folder.  
Steps for Linux
1. Make sure you have python 2.17.
2. Create a virtual environmet that uses python 2.
3. Follow download and installation instructions for mbed cli.
4. After activating the virtual env, move to the IHU top directory.
5. Enter `mbed new .` This will download all of the mbed OS files.
6. Use `mbed target` and `mbed toolchain` to set your board and compiler.
7. Type `mbed compile`.
8. Plug in board and open a serial connection.
9. Use `sudo st-flash write BUILD/<YOUR BOARD>/<TOOLCHAIN AND PROFILE>/CougSat1-IHU.bin 0x8000000` 
   to flash the board. Or you can drag and drop, but I've found this to be more reliable.
   
Note:  
If you get an error while flashing the board, unplug and replug the board. Make sure to close the serial connection before doing that however.
