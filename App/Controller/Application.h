#pragma once

#include <Model/EditorState.h>
#include <View/UIManager.h>
#include <Window/Window.h>
#include <Renderer/Renderer.h>

class Application {
public:
    Application();

    void run();

private:
    Window      m_window;
    Renderer    m_renderer;
    EditorState m_editorState;
    UIManager   m_ui;
};
