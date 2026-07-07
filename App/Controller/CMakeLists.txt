add_library(AppController OBJECT Controller.cpp Commands/FilterCommands.cpp Commands/EffectCommands.cpp)

target_link_libraries(AppController
        PUBLIC
        AppModel
        PRIVATE
        Processing
)

target_include_directories(AppController PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
