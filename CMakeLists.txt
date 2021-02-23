cmake_minimum_required(VERSION 3.17)

project(SnapshotMaker VERSION 1.0.0 DESCRIPTION "video snapshot maker" LANGUAGES C)

set_target_properties(PROPERTIES VERSION ${PROJECT_VERSION})

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
        libavdevice
        libavfilter
        libavformat
        libavcodec
        libswresample
        libswscale
        libavutil
        libpng
        libjpeg
        )

add_library(${PROJECT_NAME}Lib STATIC
        src/image.c         include/image.h
        src/path.c          include/path.h
        src/snapshot.c      include/snapshot.h
        stb/stb_image_write.h
        )

add_executable(${PROJECT_NAME}
        main.c
        )

target_link_libraries(${PROJECT_NAME} -lm
        PkgConfig::LIBAV
        ${PROJECT_NAME}Lib
        )
