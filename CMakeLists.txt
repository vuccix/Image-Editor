cmake_minimum_required(VERSION 3.20)
project(image_editor)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -pedantic -O2)

add_executable(image_editor main.cpp
        effects/src/convert.cpp
        utils/src/loader.cpp
        utils/loader.h
        effects/effects.h
        effects/src/simple.cpp
        effects/src/seam_carving.cpp
        libs/src/stb_implementation.cpp
        effects/src/blur.cpp
        utils/src/convolution.cpp
        utils/convolution.h
        effects/src/canny.cpp
        utils/src/sobel.cpp
        utils/sobel.h
)
