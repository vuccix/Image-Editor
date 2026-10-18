add_library(p++_appView OBJECT)
add_library(Paint++::AppView ALIAS p++_appView)

target_sources(p++_appView
        PRIVATE
        UIManager.cpp
        UIMenuBar.cpp
        UIToolbar.cpp
        UIPropertiesPanel.cpp
        Utils.cpp
        UI/EditorThemes.cpp
        UI/UI.cpp
)

target_include_directories(p++_appView
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
)

# internal
target_link_libraries(p++_appView
        PRIVATE
        Paint++::Renderer
        Paint++::AppModel
        Paint++::AppController
        Paint++::ImageIO
        ProjectWarnings
        PUBLIC
        Paint++::Assets
)

# external
target_link_libraries(p++_appView
        PRIVATE
        imgui
        glm
        glfw
        nfd
)
