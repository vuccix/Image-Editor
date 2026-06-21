add_executable(Editor main.cpp)

# TODO: enable
#target_compile_options(Editor PRIVATE
#        $<$<CONFIG:Debug>:-O0 -g>
#        $<$<CONFIG:Release>:-O3>
#)

target_include_directories(Editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)
