#pragma once

#include <Assets/AssetManager.h>
#include <functional>

class Renderer;
class EditorState;
class Controller;

class UIManager {
public:
    explicit UIManager(Renderer& renderer);

    // renders UI and modifies state based on user input
    void render(EditorState& state, Controller& controller, const std::function<void()>& onQuitRequest);

private:
    AssetManager m_assets;
    Renderer&    m_renderer;
    uint8_t      m_theme    = 0;

private:
    void drawToolbar(EditorState& state);
    void drawPropertiesPanel(EditorState& state);
    void drawMenuBar(EditorState& state, Controller& controller, const std::function<void()>& onQuitRequest);
    void drawCanvas(EditorState& state);

    void setTheme(uint8_t theme);
};
