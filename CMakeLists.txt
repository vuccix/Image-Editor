cmake_minimum_required(VERSION 4.1)
project(ImageEditor LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD           23)
set(CMAKE_CXX_STANDARD_REQUIRED  ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

add_compile_options(-Wall -Wpedantic -Wextra -Werror)
add_compile_options(-g -O3) # TODO: remove later

option(BUILD_SHARED_LIBS OFF)

find_package(OpenMP REQUIRED)

add_subdirectory(External)
add_subdirectory(Libs)
add_subdirectory(App)
