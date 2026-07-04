add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)

add_executable(Editor main.cpp)

target_link_libraries(Editor
        PRIVATE
        AppController
        AppView
        AppModel
)

## TODO: enable
##target_compile_options(Editor PRIVATE
##        $<$<CONFIG:Debug>:-O0 -g>
##        $<$<CONFIG:Release>:-O3>
##)
