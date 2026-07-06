add_library(AppController OBJECT Controller.cpp)

target_link_libraries(AppController
        PUBLIC
        AppModel
)

target_include_directories(AppController PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
