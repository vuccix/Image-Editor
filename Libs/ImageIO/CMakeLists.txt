add_library(p++_imageIO)
add_library(Paint++::ImageIO ALIAS p++_imageIO)

target_sources(p++_imageIO
        PRIVATE
        src/ImageIO.cpp
)

target_include_directories(p++_imageIO
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# internal
target_link_libraries(p++_imageIO
        PRIVATE
        Paint++::Canvas
        ProjectWarnings
)

# external
target_link_libraries(p++_imageIO
        PRIVATE
        stb
)
