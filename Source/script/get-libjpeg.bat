@ECHO OFF

:: This batch script downloads the 'libjpeg' library.
:: This script must be executed from the 'script' folder.

:: Settings.
SET Repository=https://github.com/vault-thirteen/libjpeg.git
SET LibFolder=libjpeg
SET LibArch=x64
SET LibVersion=9f
SET ScriptFolder=script

:: Download the Code.
git clone %Repository%

:: Prepare the code.
CD ..
MKDIR "%LibFolder%"
MKDIR "%LibFolder%\"
MKDIR "%LibFolder%\bin"
MKDIR "%LibFolder%\include"
MKDIR "%LibFolder%\include\%LibFolder%"
MKDIR "%LibFolder%\lib"

MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\%LibArch%\jpeg.dll" "%LibFolder%\bin\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\%LibArch%\jpeg.lib" "%LibFolder%\lib\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\include\*.h" "%LibFolder%\include\%LibFolder%\"
RMDIR /Q /S "%ScriptFolder%\%LibFolder%"

CD "%ScriptFolder%"
