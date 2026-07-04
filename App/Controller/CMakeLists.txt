add_library(AppController OBJECT Application.cpp)

target_link_libraries(AppController
        PUBLIC
        Renderer
        Window
        AppModel
        AppView
)

target_include_directories(AppController PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
