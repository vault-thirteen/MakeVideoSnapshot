# MakeVideoSnapshot

A simple tool to make snapshots of a video file.

For now, it only makes a single snapshot of the first video frame of the best
video stream of the file.

## Building

* Install the latest version of CMake tool.
    * https://cmake.org/


* Install one or more IDEs with support for CMake:
    * `Visual Studio` by Microsoft.
        * CMake support in Visual Studio must be installed as a separate feature.
    * `CLion` by JetBrains;
        * CMake support is already built into CLion.


* Get the latest stable shared library of the `FFmpeg` project.
    * https://ffmpeg.org/download.html


* Update all FFmpeg-related files from the downloaded FFmpeg build.
    * DLL libraries, H (header) files and LIB files for:
        * AVCodec,
        * AVFormat,
        * AVUtil,
        * SWScale.
    * They should be located in the `ffmpeg` folder.


* Get the `libjpeg` library.
    * https://ijg.org/
    * Or use the provided `get-libjpeg.bat` script.


* Get the `libpng` library.
    * http://www.libpng.org/pub/png/libpng.html
    * Or use the provided `get-libpng.bat` script.


* Get the `sdb` library.
    * https://github.com/nothings/stb
    * Or use the provided `get-stb.bat` script.


* Get the `zlib` library.
    * https://www.zlib.net/
    * Or use the provided `get-zlib.bat` script.


* Build the project using Visual Studio directly or via CLion.  
  Why ? There are many reasons. The first one is because MinGW is not able to  
  properly read object files (`.obj`) built for Microsoft Windows systems.

### Building in Visual Studio

* Open CMake GUI, set folders, configure & generate files for Visual Studio.
* Install the CMake feature for Visual Studio.
* Open the created solution in Visual Studio and build it.

### Building in CLion

* Install Microsoft Visual Studio 2022 (Community Edition).
* Install CLion and open the project in it.
* Open: `Settings -> Build, Execution, Deployment -> Toolchains`.
    * Add the Visual Studio toolchain and make it default.
* Open: `Settings -> Build, Execution, Deployment -> CMake`.
    * Disable all the profiles related to MinGW.
    * Add two profiles for Visual Studio: Release & Debug.
* Click: `Tool -> CMake -> Reset Cache and Reload Project`.
    * In case of an error about `CMAKE_MAKE_PROGRAM` which is not found:
        * Install MinGW, including its version of the `make` tool,
        * Add MinGW's `bin` folder to your system's `PATH` environment variable.
* Build the project.

## Installing

TODO
