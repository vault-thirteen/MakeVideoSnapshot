cmake_minimum_required(VERSION 3.28)
project(MakeVideoSnapshot VERSION 1.0.0 DESCRIPTION "Video Snapshot Maker" LANGUAGES C)
set_target_properties(PROPERTIES VERSION ${PROJECT_VERSION})
set(CMAKE_C_STANDARD 23)

add_executable(${PROJECT_NAME} image.c main.c mvs.h snapshot.c std.h std.c)

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/ffmpeg/lib/avcodec.lib")
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ffmpeg/bin/avcodec-60.dll")

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/ffmpeg/lib/avformat.lib")
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ffmpeg/bin/avformat-60.dll")

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/ffmpeg/lib/avutil.lib")
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ffmpeg/bin/avutil-58.dll")

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/ffmpeg/lib/swresample.lib")
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ffmpeg/bin/swresample-5.dll")

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/ffmpeg/lib/swscale.lib")
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ffmpeg/bin/swscale-7.dll")

add_library(libjpeg SHARED IMPORTED)
set_target_properties(libjpeg PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/libjpeg/lib/jpeg.lib")
set_target_properties(zlib PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libjpeg/bin/jpeg.dll")

add_library(libpng SHARED IMPORTED)
set_target_properties(libpng PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/libpng/lib/libpng16.lib")
set_target_properties(libpng PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libpng/bin/libpng16.dll")

add_library(zlib SHARED IMPORTED)
set_target_properties(zlib PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/zlib/lib/zlib.lib")
set_target_properties(zlib PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/zlib/bin/zlib.dll")

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/ffmpeg/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libpng/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/zlib/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libjpeg/include")

target_link_libraries(${PROJECT_NAME} avcodec avformat avutil swresample swscale libjpeg libpng zlib)

# Debugging.
get_property(IncludeFolders TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "IncludeFolders='${IncludeFolders}'")
