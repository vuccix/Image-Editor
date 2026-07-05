#pragma once

#include <Model/EditorState.h>
#include <functional>

class Renderer;

class UIManager {
public:
    explicit UIManager(Renderer& renderer);

    // renders UI and modifies state based on user input
    void render(EditorState& state, const std::function<void()>& onQuitRequest);

private:
    Renderer& m_renderer;
    uint8_t   m_theme    = 0;

private:
    void drawToolbar(EditorState& state);
    void drawPropertiesPanel(EditorState& state);
    void drawMenuBar(EditorState& state, const std::function<void()>& onQuitRequest);
    void drawCanvas(EditorState& state);

    void setTheme(uint8_t theme);
};
