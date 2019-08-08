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