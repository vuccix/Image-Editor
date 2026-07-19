add_library(Serialization STATIC src/Serialize.cpp)

target_link_libraries(Serialization
        PRIVATE
        nfd
        stb
        Canvas
)

target_include_directories(Serialization PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
