add_subdirectory(Model)
add_subdirectory(View)
add_subdirectory(Controller)

add_executable(Paint++ WIN32)

target_sources(Paint++
        PRIVATE
        main.cpp
        Application.cpp
)

target_link_libraries(Paint++
        PRIVATE
        Paint++::AppController
        Paint++::AppView
        Paint++::AppModel
        Paint++::Window
        Paint++::Renderer
        ProjectWarnings
)
