# Make Video Snapshot

A tool to make snapshots of a video file.

This tool supports a lot of video formats and works in a console mode.

## <a name="section_0">Contents</a>

- [Contents](#section_0)
- [Input (Video) Formats](#section_1)
- [Output (Image) Formats](#section_2)
- [Building](#section_3)
- [Installing](#section_4)
- [Usage](#section_5)
- [Version & Dependency](#section_6)
- [Build Information](#section_7)
- [Feedback](#section_8)

## <a name="section_1">Input Video Format</a>

This tool supports all the video formats, supported by the `FFmpeg` library.
However, sometimes there may be some difficulty with specific format, as the
`FFmpeg` library is known to use the most default decoders for video files.

This tool uses `libvpx-vp9` decoder for Google's VP9 video files to decode
transparency correctly. Some other decoders may detect video stream index and
pixel format incorrectly. For such cases this tool provides a method to set
stream index and pixel format manually. More information about this is
provided in the `Usage` section of this file.

## <a name="section_2">Output Image Format</a>

This tool can export snapshots as images in PNG and JPEG formats. More 
information about output image formats is provided in the description of the 
`Writer` parameter in the `Usage` section of this file.

## <a name="section_3">Building</a>

* Install the latest version of `CMake` tool.
    * https://cmake.org/


* Install one or more IDEs with support for `CMake`:
    * `Visual Studio` by Microsoft.
        * CMake support in Visual Studio must be installed as a separate feature.
    * `CLion` by JetBrains;
        * CMake support is already built into CLion.


* Get the stable shared library of the `FFmpeg` project.
    * https://ffmpeg.org/download.html


* Update all files related to `FFmpeg` from the downloaded `FFmpeg` build.
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


* Get the `stb` library.
    * https://github.com/nothings/stb
    * Or use the provided `get-stb.bat` script.


* Get the `zlib` library.
    * https://www.zlib.net/
    * Or use the provided `get-zlib.bat` script.


* Build the project using `Visual Studio` directly or via `CLion`.  
  Why ? There are many reasons. The first one is because `MinGW` is not able to  
  properly read object files (`.obj`) built for `Microsoft Windows` systems.


* Versions of dependencies are described in [Version & Dependency](#section_6) section.

### Building in Visual Studio

* Open `CMake` GUI, set folders, configure & generate files for `Visual Studio`.
* Install the `CMake` feature for `Visual Studio`.
* Open the created solution in `Visual Studio` and build it.

### Building in CLion

* Install `Microsoft Visual Studio 2022 (Community Edition)`.
* Install `CLion` and open the project in it.
* Open: `Settings -> Build, Execution, Deployment -> Toolchains`.
    * Add the Visual Studio toolchain and make it default.
* Open: `Settings -> Build, Execution, Deployment -> CMake`.
    * Disable all the profiles related to `MinGW`.
    * Add two profiles for `Visual Studio`: Release & Debug.
* Click: `Tool -> CMake -> Reset Cache and Reload Project`.
    * In case of an error about `CMAKE_MAKE_PROGRAM` which is not found:
        * Install `MinGW`, including its version of the `make` tool,
        * Add the `bin` folder of `MinGW` to your system's `PATH` environment variable.
* Build the project.

## <a name="section_4">Installing</a>

* Make DLL files of `FFmpeg` library accessible to the program:
    * either install the shared version of `FFmpeg` and add
      it's `bin` folder to `PATH` environment variable
    * or copy its DLL files into the program's directory


* Make DLL files of `libpng` library accessible to the program:
    * either install the `libpng` library and make it visible
      via `PATH` environment variable
    * or copy its DLL files into the program's directory


* Make DLL files of `zlib` library accessible to the program:
    * either install the `zlib` library and make it visible
      via `PATH` environment variable
    * or copy its DLL files into the program's directory

List of required DLL files is described in [Version & Dependency](#section_6) section.

## <a name="section_5">Usage</a>

The program works in console mode and requires six (6) command line arguments:

`<executable> <input_file> <stream_index> <pixel_format_id> <output_folder> <writer> <nth>`

1. **Input Video File**.  
   Path to the video file from which you want to make snapshots.


2. **Video Stream Index**.  
   This parameter is optional and means an index of a video stream to capture.  
   Numbers are starting from 0, i.e. stream #1 is index 0.  
   Hint: `-1` means `auto`, values >= 0 set an exact stream index.  
   In most cases the automatic `-1` is enough.


3. **Pixel Format ID**.  
   This parameter is optional and means an ID of pixel format.  
   The list of possible IDs is available in header files of FFmpeg.  
   Hint: `-1` means `auto`, values >= 0 set an exact pixel format.  
   In most cases the automatic `-1` is enough.


4. **Output Folder**.  
   Path to the folder where you want your snapshots to be placed.  
   Names of the snapshots will be in format `N.xxx`,  
   where `N` is the frame number and `xxx` is the file extension.


5. **Writer**.  
   Image encoder.  
   Possible values are following:
    * `libpng`
        * PNG format with the best compression and alpha channel
    * `libjpeg`
        * JPEG format with the best quality (100)
    * `stb`
        * PNG format with no compression, but with an alpha channel


6. **N-th**.  
   N-th frames to capture.  
   If set as 0 or 1, all the frames will be captured.  
   If set to N, where N is > 1, then each N-th frame will be captured.  
   For example, if N = 10, then frames 10, 20, 30, ... will be captured.  
   Note that the first frame is always captured independently of the setting.

**Usage Example**
<pre>MakeVideoSnapshot.exe D:\Videos\video.mp4 -1 -1 D:\Pictures libpng 10</pre>

## <a name="section_6">Version & Dependency</a>

This tool does not have its own strict versioning.

This tool was built using following libraries.

* **FFmpeg** version `7.0.1`  
  DLL Files: avcodec-61.dll, avformat-61.dll, avutil-59.dll, swscale-8.dll.


* **libjpeg** version `9f`  
  DLL Files: none, it is built-into as a static library.


* **libpng** version `1.6.43`  
  DLL Files: libpng16.dll.


* **zlib** version `1.3.1`  
  DLL Files: zlib.dll.


* **stb**, no versioning  
  DLL Files: none.

## <a name="section_7">Build Information</a>

[Link](./Build/ReadMe.md).

## <a name="section_8">Feedback</a>

If you have found a bug, have comments or want to suggest a new feature to be
implemented, feel free to open an issue in this GitHub repository.
