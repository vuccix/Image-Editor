file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_library(Processing STATIC ${SOURCES})

target_include_directories(Processing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Processing
        PRIVATE
        Canvas
        OpenMP::OpenMP_CXX
)
