add_library(p++_window)
add_library(Paint++::Window ALIAS p++_window)

target_sources(p++_window
        PRIVATE
        src/Window.cpp
)

target_include_directories(p++_window
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# internal
target_link_libraries(p++_window
        PRIVATE
        ProjectWarnings
)

# external
target_link_libraries(p++_window
        PRIVATE
        glad
        glfw
)
