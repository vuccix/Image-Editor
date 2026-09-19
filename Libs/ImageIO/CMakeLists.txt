add_library(ImageIO STATIC src/ImageIO.cpp)

target_link_libraries(ImageIO
        PRIVATE
        stb
        Canvas
)

target_include_directories(ImageIO PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
