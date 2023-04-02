@ECHO OFF

:: This batch script downloads the 'stb' library.
:: This script must be executed from the 'script' folder.

:: Settings.
SET Repository=https://github.com/nothings/stb.git
SET LibFolder=stb
SET ScriptFolder=script

:: Download the Code.
git clone %Repository%

:: Prepare the code.
MOVE "%LibFolder%" ..\
CD "..\%LibFolder%"

RMDIR /Q /S ".git"
RMDIR /Q /S ".github"
RMDIR /Q /S "data"
RMDIR /Q /S "deprecated"
RMDIR /Q /S "docs"
RMDIR /Q /S "tests"
RMDIR /Q /S "tools"
DEL ".travis.yml"

CD "%ScriptFolder%"
