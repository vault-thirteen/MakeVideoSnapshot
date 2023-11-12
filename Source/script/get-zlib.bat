@ECHO OFF

:: This batch script downloads the 'zlib' library.
:: This script must be executed from the 'script' folder.

:: Settings.
SET Repository=https://github.com/vault-thirteen/zlib.git
SET LibFolder=zlib
SET LibVersion=1.3
SET ScriptFolder=script

:: Download the Code.
git clone %Repository%

:: Prepare the code.
CD ..
MKDIR "%LibFolder%"
MKDIR "%LibFolder%\include"
MKDIR "%LibFolder%\include\%LibFolder%"

MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\zlib.dll" "%LibFolder%\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\zlib.exp" "%LibFolder%\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\zlib.lib" "%LibFolder%\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\zlibstatic.lib" "%LibFolder%\"
MOVE "%ScriptFolder%\%LibFolder%\Build\%LibVersion%\include\*.h" "%LibFolder%\include\%LibFolder%\"
RMDIR /Q /S "%ScriptFolder%\%LibFolder%"

CD "%ScriptFolder%"
