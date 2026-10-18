add_library(p++_appController OBJECT)
add_library(Paint++::AppController ALIAS p++_appController)

target_sources(p++_appController
        PRIVATE
        Controller.cpp
        Commands/Names.cpp
        Commands/LayerCommand.cpp
        Commands/CanvasCommand.cpp
        Commands/FunctionalCommand.cpp
        Commands/StatefulCommand.cpp
)

target_include_directories(p++_appController
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(p++_appController
        PUBLIC
        Paint++::AppModel
        PRIVATE
        Paint++::Processing
        ProjectWarnings
)
