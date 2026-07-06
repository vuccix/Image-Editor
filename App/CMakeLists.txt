add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)

add_executable(Editor main.cpp Application.cpp)

target_link_libraries(Editor
        PRIVATE
        AppController
        AppView
        AppModel
        Window
        Renderer
)

## TODO: enable
##target_compile_options(Editor PRIVATE
##        $<$<CONFIG:Debug>:-O0 -g>
##        $<$<CONFIG:Release>:-O3>
##)
