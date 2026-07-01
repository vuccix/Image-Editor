add_library(Renderer STATIC src/Renderer.cpp)

target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Renderer
        PRIVATE
        glad
        Canvas
)
