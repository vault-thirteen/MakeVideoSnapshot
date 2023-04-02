# MakeVideoSnapshot

A simple library to make snapshots from a video file.

For now, it only makes a single snapshot of the first video frame of the best 
video stream of the file. 

## Building

* Get the IDE with support for CMake. 
  * `CLion` by JetBrains;
  * `Visual Studio` by Microsoft.


* Get the latest shared library of the `FFmpeg` project.
  * https://ffmpeg.org/download.html


* Update all FFmpeg-related files from the downloaded FFmpeg build.
  * They are located in the `ffmpeg` folder.


* Get the `sdb` library.
  * https://github.com/nothings/stb 
  * Use the provided `get-stb.bat` script.


* Get the `libpng` library.
  * http://www.libpng.org/pub/png/libpng.html
  * Use the provided `get-libpng.bat` script.


* Get the `libjpeg` library.
  * https://ijg.org/
  * Use the provided `get-libjpeg.bat` script.
