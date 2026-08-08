add_library(AppView OBJECT
        UIManager.cpp
        UIMenuBar.cpp
        UIToolbar.cpp
        UIPropertiesPanel.cpp
        Utils.cpp
        UI/EditorThemes.cpp
        UI/UI.cpp
)

target_link_libraries(AppView
        PRIVATE
        imgui
        glm
        glfw
        Renderer
        AppModel
        AppController
        Serialization
        PUBLIC
        Assets
)

target_include_directories(AppView PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
