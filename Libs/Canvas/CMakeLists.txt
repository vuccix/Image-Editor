add_library(p++_canvas)
add_library(Paint++::Canvas ALIAS p++_canvas)

target_sources(p++_canvas
        PRIVATE
        src/Canvas.cpp
        src/Layer.cpp
        src/Pixel.cpp
)

target_include_directories(p++_canvas
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# internal
target_link_libraries(p++_canvas
        PRIVATE
        ProjectWarnings
)

# external
target_link_libraries(p++_canvas
        PRIVATE
        OpenMP::OpenMP_CXX
)
