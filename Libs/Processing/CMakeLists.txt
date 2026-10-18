add_library(p++_processing)
add_library(Paint++::Processing ALIAS p++_processing)

target_sources(p++_processing
        PRIVATE
        src/Blur.cpp
        src/EdgeDetection.cpp
        src/Effects.cpp
        src/Filters.cpp
        src/Noise.cpp
        src/SeamCarving.cpp
        src/Utils.cpp
)

target_include_directories(p++_processing
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# internal
target_link_libraries(p++_processing
        PRIVATE
        Paint++::Canvas
        ProjectWarnings
)

# external
target_link_libraries(p++_processing
        PRIVATE
        OpenMP::OpenMP_CXX
)
