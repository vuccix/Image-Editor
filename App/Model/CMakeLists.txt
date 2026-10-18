add_library(p++_appModel OBJECT)
add_library(Paint++::AppModel ALIAS p++_appModel)

target_sources(p++_appModel
        PRIVATE
        EditorState.cpp
)

target_include_directories(p++_appModel
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
)

# internal
target_link_libraries(p++_appModel
        PUBLIC
        Paint++::Canvas
        PRIVATE
        ProjectWarnings
)

# external
target_link_libraries(p++_appModel
        PUBLIC
        glm
)
