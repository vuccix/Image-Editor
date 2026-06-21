target_sources(Editor PRIVATE Application.cpp)

target_link_libraries(Editor
        PRIVATE
        Window
        Renderer
)
