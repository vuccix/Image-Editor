#pragma once

#include <Model/EditorState.h>

class UIManager {
public:
    UIManager();

    // renders UI and modifies state based on user input
    void render(EditorState& state);

private:
    uint8_t m_selectedTheme = 0;

private:
    void drawToolbar(EditorState& state);
    void drawPropertiesPanel(EditorState& state);
    void drawMenuBar(EditorState& state);
};
