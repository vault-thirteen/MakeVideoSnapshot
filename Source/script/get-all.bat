@ECHO OFF

:: This batch script runs all other get-scripts.
:: This script must be executed from the 'script' folder.

ECHO:
ECHO Step 1. FFmpeg.
CALL get-ffmpeg.bat
IF %ErrorLevel% NEQ 0 EXIT /b %ErrorLevel%

ECHO:
ECHO Step 2. libjpeg.
CALL get-libjpeg.bat
IF %ErrorLevel% NEQ 0 EXIT /b %ErrorLevel%

ECHO:
ECHO Step 3. libpng.
CALL get-libpng.bat
IF %ErrorLevel% NEQ 0 EXIT /b %ErrorLevel%

ECHO:
ECHO Step 4. stb.
CALL get-stb.bat
IF %ErrorLevel% NEQ 0 EXIT /b %ErrorLevel%

ECHO:
ECHO Step 5. zlib.
CALL get-zlib.bat
IF %ErrorLevel% NEQ 0 EXIT /b %ErrorLevel%

ECHO:
ECHO SUCCESS
