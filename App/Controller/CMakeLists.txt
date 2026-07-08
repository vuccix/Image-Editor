add_library(AppController OBJECT Controller.cpp Commands/LayerCommand.cpp Commands/CanvasCommand.cpp)

target_link_libraries(AppController
        PUBLIC
        AppModel
        PRIVATE
        Processing
)

target_include_directories(AppController PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
