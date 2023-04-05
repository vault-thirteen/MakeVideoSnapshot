cmake_minimum_required(VERSION 3.25)
project(MakeVideoSnapshot VERSION 1.0.0 DESCRIPTION "Video Snapshot Maker" LANGUAGES C)
set_target_properties(PROPERTIES VERSION ${PROJECT_VERSION})
set(CMAKE_C_STANDARD 23)

add_executable(${PROJECT_NAME} main.c image.c snapshot.c mvs.h)

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/ffmpeg/lib/avcodec.lib")
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ffmpeg/bin/avcodec-60.dll")

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/ffmpeg/lib/avformat.lib")
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ffmpeg/bin/avformat-60.dll")

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/ffmpeg/lib/avutil.lib")
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ffmpeg/bin/avutil-58.dll")

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/ffmpeg/lib/swscale.lib")
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ffmpeg/bin/swscale-7.dll")

add_library(libpng SHARED IMPORTED)
set_target_properties(libpng PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/libpng/lib/libpng16.lib")
set_target_properties(libpng PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libpng/bin/libpng16.dll")

add_library(zlib SHARED IMPORTED)
set_target_properties(zlib PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/zlib/zlib.lib")
set_target_properties(zlib PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/zlib/zlib.dll")

add_library(libjpeg OBJECT IMPORTED)
set_target_properties(libjpeg PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/libjpeg/jpeg.lib")
set_property(TARGET libjpeg PROPERTY IMPORTED_OBJECTS
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jaricom.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcapimin.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcapistd.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcarith.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jccoefct.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jccolor.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcdctmgr.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jchuff.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcinit.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcmainct.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcmarker.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcmaster.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcomapi.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcparam.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcprepct.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jcsample.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jctrans.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdapimin.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdapistd.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdarith.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdatadst.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdatasrc.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdcoefct.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdcolor.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jddctmgr.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdhuff.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdinput.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdmainct.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdmarker.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdmaster.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdmerge.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdpostct.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdsample.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jdtrans.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jerror.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jfdctflt.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jfdctfst.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jfdctint.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jidctflt.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jidctfst.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jidctint.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jmemmgr.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jmemnobs.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jquant1.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jquant2.obj"
        "${CMAKE_SOURCE_DIR}/libjpeg/x64/jutils.obj")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/ffmpeg/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libpng/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/zlib/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libjpeg/x64/include")

target_link_libraries(${PROJECT_NAME} avcodec avformat avutil swscale libpng zlib libjpeg)

# Debugging.
get_property(IncludeFolders TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "IncludeFolders='${IncludeFolders}'")
