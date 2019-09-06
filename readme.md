# CougSat-1 Software #
Each processor (usually each subsystem) has a folder that contains its software. It may reference common drivers or components in the CISLibrary folder.

## Setup for STM32 ##
* Install [VSCode](https://code.visualstudio.com/) (able to do any IDE but this is recommended).
* Install [ClangFormat](http://releases.llvm.org/download.html) (Part of LLVM download)
* Install the following extensions:
  * [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
  * [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) - follow instructions to setup clang-format executable location
  * [Doxygen Documentation Generator](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
  * [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
  * [Code Spell Checker](https://marketplace.visualstudio.com/items?itemName=streetsidesoftware.code-spell-checker)
* Open the folder to the target processor. Save this as a workspace if you want, you can then add the library folder to see those files too.
* PlatformIO automatically will detect the `platformio.ini` file and begin initializing itself. If you do not have the `ST STM32` embedded platform under PlatformIO home installed, during the first time you build PlatformIO will automatically download it. If there are include errors, restart VSCode (which has PlatformIO run `Rebuild IntelliSense Index`) and those will disappear.
* Run the default build task to have PlatformIO start the Mbed compiler.

## Setup for Ground Software ##
* Install [VSCode](https://code.visualstudio.com/) (able to do any IDE but this is recommended).
* Install [ClangFormat](http://releases.llvm.org/download.html) (Part of LLVM download)
* Install [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/?q=build+tools)
  * Select `Tools for Visual Studio 2019`
  * In the installer, select `C++ build tools`
  * Add `msbuild` to `PATH`
    * Default location `C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin`
* Install the following extensions:
  * [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
  * [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) - follow instructions to setup clang-format executable location
  * [Doxygen Documentation Generator](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
  * [Code Spell Checker](https://marketplace.visualstudio.com/items?itemName=streetsidesoftware.code-spell-checker)
* Open the folder to `CougSat1-Ground`. Save this as a workspace if you want.
* Initialize the submodules using `git submodule update --init --recursive`
* Run the `setupRTLSDR` script
  * This downloads the DLLs for the rtl-sdr library and opens Zadig for installing the driver for the RTL-SDR dongle
  * When Zadig opens, insert the RTL-SDR dongle, select `Bulk-In, Interface (Interface 0)`, click install WinUSB driver
* Execute the default test task which will compile everything and launch the executable