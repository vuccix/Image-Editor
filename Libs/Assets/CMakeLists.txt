add_library(p++_assets)
add_library(Paint++::Assets ALIAS p++_assets)

target_sources(p++_assets
        PRIVATE
        src/AssetManager.cpp
)

target_include_directories(p++_assets
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(p++_assets
        PUBLIC
        Paint++::Renderer
        PRIVATE
        ProjectWarnings
)
