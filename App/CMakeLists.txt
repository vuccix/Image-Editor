add_executable(Editor main.cpp)

# TODO: enable
#target_compile_options(Editor PRIVATE
#        $<$<CONFIG:Debug>:-O0 -g>
#        $<$<CONFIG:Release>:-O3>
#)

target_link_libraries(Editor
        PRIVATE
)
