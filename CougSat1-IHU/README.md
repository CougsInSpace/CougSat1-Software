# Internal Housekeeping Unit

Current status:  
Working on the FileSystem.

## Installation and Compiling

This is a very complicated process. All the files to compile are located in the CougSat1-IHU folder.  
Steps for Linux  
  
*mbed-cli says it supports python 3, but I couldn't get it to work.*
1. Make sure you have python 2.7.
2. Create a virtual environmet that uses python 2.
	a. `pip2 install virtualenv`
	b. `cd <wherever you want the venv to be>`
	c. `python2 -m virtualenv <name of virtual env>`
	d. `source <path to your virtual env folder>/bin/activate`
	f. verify python version is 2.7.xx with `python --version`
3. Follow download and installation instructions for mbed cli.
	a. If you followed the previous steps you just need:
		`pip install mbed-cli`
4. Move to the IHU top directory.
5. Enter `mbed new .` This will download all of the mbed OS files.
6. Use `mbed target` and `mbed toolchain` to set your board and compiler.
	a. Example:
		`mbed target NUCLEO_L476RG`
		`mbed toolchain GCC_ARM`
7. Type `mbed compile`.
8. Plug in board and open a serial connection.
9. Use `sudo st-flash write BUILD/<YOUR BOARD>/<TOOLCHAIN AND PROFILE>/CougSat1-IHU.bin 0x8000000` 
   to flash the board. Or you can drag and drop, but I've found this to be more reliable.  

Steps for Windows:
The steps should be similiar but I can't say for sure. Honestly you're likely better off with mbed studio or platformio.
   
Note:  
* If you get an error while flashing the board, unplug and replug the board. Make sure to close the serial connection before doing that however. You can also sometimes get it to work by using the reset button. It won't flash if the board is sleeping.  

* If you use drag and drop, the board seems to fill up and not actually delete properly. If that's the case, just use st-flash and it should clear up.  

* If the board has a flashing light after pushing your binary to it, it means it had a hard fault and you screwed up somewhere in your code. Probably you forgot to initialize some memory or have something hooked up wrong. Also watch your threads.  
