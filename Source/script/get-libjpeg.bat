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
MKDIR "%LibFolder%\%LibArch%"
MKDIR "%LibFolder%\%LibArch%\include"
MKDIR "%LibFolder%\%LibArch%\include\%LibFolder%"

MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\%LibArch%\jpeg.lib" "%LibFolder%\%LibArch%\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\%LibArch%\jpeg.pdb" "%LibFolder%\%LibArch%\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\include\*.h" "%LibFolder%\%LibArch%\include\%LibFolder%\"
RMDIR /Q /S "%ScriptFolder%\%LibFolder%"

CD "%ScriptFolder%"
