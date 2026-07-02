add_library(Canvas STATIC src/Canvas.cpp src/Layer.cpp)

target_include_directories(Canvas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

