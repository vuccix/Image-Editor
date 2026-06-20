add_library(Window STATIC src/Window.cpp)

target_include_directories(Window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Window
        PRIVATE
        glad
        glfw
)
