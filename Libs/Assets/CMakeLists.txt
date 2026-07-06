add_library(Assets STATIC src/AssetManager.cpp)

target_include_directories(Assets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Assets
        PUBLIC
        Renderer
)
