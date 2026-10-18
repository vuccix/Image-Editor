add_library(p++_renderer)
add_library(Paint++::Renderer ALIAS p++_renderer)

target_sources(p++_renderer
        PRIVATE
        src/Renderer.cpp
        src/Texture.cpp
)

target_include_directories(p++_renderer
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# internal
target_link_libraries(p++_renderer
        PRIVATE
        Paint++::Canvas
        ProjectWarnings
)

# external
target_link_libraries(p++_renderer
        PRIVATE
        glad
)
