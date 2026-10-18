cmake_minimum_required(VERSION 4.1)
project(Paint++ LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD           23)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_C_STANDARD             11)
set(CMAKE_C_STANDARD_REQUIRED    ON)
set(CMAKE_C_EXTENSIONS          OFF)

include(CMake/ProjectWarnings.cmake)

add_compile_options(-g -O3 -fopenmp) # TODO: remove later
add_link_options(-fopenmp)

find_package(OpenMP COMPONENTS C CXX REQUIRED)

option(BUILD_SHARED_LIBS OFF)

add_subdirectory(External)
add_subdirectory(Libs)
add_subdirectory(App)
