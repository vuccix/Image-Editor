#pragma once

#include <Model/EditorState.h>

class UIManager {
public:
    explicit UIManager(void* window);

    // renders UI and modifies state based on user input
    void render(EditorState& state);

private:
    void drawToolbar(EditorState& state);
    void drawPropertiesPanel(EditorState& state);
    void drawMenuBar(EditorState& state);
};
