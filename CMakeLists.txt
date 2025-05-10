cmake_minimum_required(VERSION 3.30)
project(image_editor)

set(CMAKE_CXX_STANDARD 20)

add_executable(image_editor main.cpp
        effects/src/convert.cpp
        utils/src/loader.cpp
        utils/loader.h
        effects/effects.h
        effects/src/simple.cpp
        effects/src/seam_carving.cpp
)
