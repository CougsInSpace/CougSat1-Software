# CougSat-1 Software #
Each processor (usually each subsystem) has a folder that contains its software. It may reference common drivers or components in the CISLibrary folder.

## Setup for STM32 ##
* Install [VSCode](https://code.visualstudio.com/) (able to do any IDE but this is recommended).
* Install [ClangFormat](http://releases.llvm.org/download.html) (Part of LLVM download, see pre-built binaries) Current version is LLVM 9.0.0 go to pre-built binary and click on the appropriate link based on your OS.
* Install the following extensions (to download extension click on the extension tab on the middle left side):
  
  * [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
  * [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) - follow instructions to setup clang-format executable location
  * [Doxygen Documentation Generator](https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen)
  * [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
  * [Code Spell Checker](https://marketplace.visualstudio.com/items?itemName=streetsidesoftware.code-spell-checker)
* Copy `.githooks/pre-commit` to `.git/hooks/pre-commit` using `cp .githooks/pre-commit .git/hooks/pre-commit` or manually via the following steps

1. In file explorer click on view then check mark hidden files.
2.  Go into the `CougSat1-Software/.githooks` folder and copy the precommit file.
3. Go back and then click on the `.git/hooks` folder then paste the file in there.
* Open the folder to the target processor. Save this as a workspace if you want, you can then add the library folder to see those files too. You can do this by clicking file/open folder. In file explorer make sure your are in Cougsat1-software then click on project you are working on. If you are modifying the CISLibrary, add this folder to the workspace as well.

* PlatformIO automatically will detect the `platformio.ini` file and begin initializing itself. If you do not have the `ST STM32` embedded platform under PlatformIO home installed, during the first time you build PlatformIO will automatically download it. If there are include errors, restart VSCode (which has PlatformIO run `Rebuild IntelliSense Index`) and those will disappear.
* Run the default build task to have PlatformIO start the Mbed compiler.

## Setup for Ground Software ##
* Install [VSCode](https://code.visualstudio.com/) (able to do any IDE but this is recommended).
* Install [ClangFormat](http://releases.llvm.org/download.html) (Part of LLVM download, see pre-built binaries) Current version is LLVM 9.0.0 go to pre-built binary and click on the appropriate link based on your OS.
* Install [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/?q=build+tools)
  * Select `Tools for Visual Studio 2019`
  * In the installer, select `C++ build tools`
  * Add `msbuild` to `PATH`
    * Default location `C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\MSBuild\Current\Bin`
* Install [Visual C++ 2010 SP1](https://www.microsoft.com/en-US/download/confirmation.aspx?id=8328)
* Install the following extensions:
  * [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
  * [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) - follow instructions to setup clang-format executable location
  * [Doxygen Documentation Generator](https://marketplace.visualstudio.com/items?itemName=cschlosser.doxdocgen)
  * [Code Spell Checker](https://marketplace.visualstudio.com/items?itemName=streetsidesoftware.code-spell-checker)
* Copy `.githooks/pre-commit` to `.git/hooks/pre-commit` using `cp .githooks/pre-commit .git/hooks/pre-commit` or manually via the following steps

1. In file explorer click on view then check mark hidden files.
2.  Go into the `CougSat1-Software/.githooks` folder and copy the precommit file.
3. Go back and then click on the `.git/hooks` folder then paste the file in there.
* Open the folder to the target processor. Save this as a workspace if you want, you can then add the library folder to see those files too. You can do this by clicking file/open folder. In file explorer make sure your are in Cougsat1-software then click on project you are working on. If you are modifying the CISLibrary, add this folder to the workspace as well.
* Open the folder to `CougSat1-Ground`. Save this as a workspace if you want. If you are modifying the CISLibrary, add this folder to the workspace as well.
* Initialize the submodules using `git submodule update --init --recursive`
* Run the `setupRTLSDR` script
  * This downloads the DLLs for the rtl-sdr library and opens Zadig for installing the driver for the RTL-SDR dongle
  * When Zadig opens, insert the RTL-SDR dongle, select `Bulk-In, Interface (Interface 0)`, click install WinUSB driver
* Execute the default test task which will compile everything and launch the executable