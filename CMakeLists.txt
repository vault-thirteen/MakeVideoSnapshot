cmake_minimum_required(VERSION 3.17)
project(SnapshotMaker C)

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

add_executable(${PROJECT_NAME}
        main.c
        image.c image.h
        path.c path.h
        snapshot.c snapshot.h
        stb/stb_image_write.h)

target_link_libraries(${PROJECT_NAME} -lm
        PkgConfig::LIBAV
        )
