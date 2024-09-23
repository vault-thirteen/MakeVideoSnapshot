@ECHO OFF

:: This batch script copies required files of the 'FFmpeg' library.
:: 'FFmpeg' must be already downloaded and extracted to its folder.
:: This script must be executed from the 'script' folder.

:: Settings.
SET FFmpegFolder=C:\Tools\ffmpeg-7.0.2-full_build-shared
SET LibFolder=ffmpeg
SET ScriptFolder=script

:: Download the Code.
:: Do it manually.
IF EXIST "%FFmpegFolder%\" (ECHO FFmpeg is found) ELSE (EXIT /B 1)

:: Prepare the code.
CD ..
MKDIR "%LibFolder%"
MKDIR "%LibFolder%\"
MKDIR "%LibFolder%\bin"
MKDIR "%LibFolder%\include"
MKDIR "%LibFolder%\include\libavcodec"
MKDIR "%LibFolder%\include\libavformat"
MKDIR "%LibFolder%\include\libavutil"
MKDIR "%LibFolder%\include\libswresample"
MKDIR "%LibFolder%\include\libswscale"
MKDIR "%LibFolder%\lib"

COPY "%FFmpegFolder%\bin\avcodec-61.dll" "%LibFolder%\bin\"
COPY "%FFmpegFolder%\bin\avformat-61.dll" "%LibFolder%\bin\"
COPY "%FFmpegFolder%\bin\avutil-59.dll" "%LibFolder%\bin\"
COPY "%FFmpegFolder%\bin\swresample-5.dll" "%LibFolder%\bin\"
COPY "%FFmpegFolder%\bin\swscale-8.dll" "%LibFolder%\bin\"

COPY "%FFmpegFolder%\lib\avcodec.lib" "%LibFolder%\lib\"
COPY "%FFmpegFolder%\lib\avformat.lib" "%LibFolder%\lib\"
COPY "%FFmpegFolder%\lib\avutil.lib" "%LibFolder%\lib\"
COPY "%FFmpegFolder%\lib\swresample.lib" "%LibFolder%\lib\"
COPY "%FFmpegFolder%\lib\swscale.lib" "%LibFolder%\lib\"

COPY "%FFmpegFolder%\include\libavcodec\*.h" "%LibFolder%\include\libavcodec\"
COPY "%FFmpegFolder%\include\libavformat\*.h" "%LibFolder%\include\libavformat\"
COPY "%FFmpegFolder%\include\libavutil\*.h" "%LibFolder%\include\libavutil\"
COPY "%FFmpegFolder%\include\libswresample\*.h" "%LibFolder%\include\libswresample\"
COPY "%FFmpegFolder%\include\libswscale\*.h" "%LibFolder%\include\libswscale\"

CD "%ScriptFolder%"
