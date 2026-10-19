@ECHO OFF

:: This batch script removes folders of third party libraries.
:: This script must be executed from the 'script' folder.

SET ScriptFolder=script

CD ..
RMDIR /Q /S "ffmpeg"
RMDIR /Q /S "libjpeg"
RMDIR /Q /S "libpng"
RMDIR /Q /S "stb"
RMDIR /Q /S "zlib"
CD "%ScriptFolder%"
