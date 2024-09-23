@ECHO OFF

:: This batch script downloads the 'libpng' library.
:: This script must be executed from the 'script' folder.

:: Settings.
SET Repository=https://github.com/vault-thirteen/libpng.git
SET LibFolder=libpng
SET LibVersion=1.6.44
SET ScriptFolder=script

:: Download the Code.
git clone %Repository%
IF %ErrorLevel% NEQ 0 EXIT /b %ErrorLevel%

:: Prepare the code.
CD ..
MKDIR "%LibFolder%"
MKDIR "%LibFolder%\bin"
MKDIR "%LibFolder%\include"
MKDIR "%LibFolder%\include\%LibFolder%"
MKDIR "%LibFolder%\lib"

MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\libpng*.dll" "%LibFolder%\bin\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\libpng*.lib" "%LibFolder%\lib\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\include\*.h" "%LibFolder%\include\%LibFolder%\"
RMDIR /Q /S "%ScriptFolder%\%LibFolder%"

CD "%ScriptFolder%"
