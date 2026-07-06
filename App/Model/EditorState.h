#pragma once

#include <Canvas/Canvas.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

enum class ActiveTool {
    Brush, Pencil, Spray,
    Eraser,
    Fill, Pick,
    Text,
    BoxSelect, MagicSelect, LassoSelect,
    Line, Curve,
};

struct EditorState {
    ActiveTool activeTool      = ActiveTool::Brush;
    float      brushSize       = 10.f;
    glm::vec4  primaryColor    = { 1.f, 1.f, 1.f, 1.f };
    glm::vec4  secondaryColor  = { 0.f, 0.f, 0.f, 1.f };

    bool       requestSave     = false;
    bool       requestLoad     = false;

    float      zoomLevel       = 1.f;
    glm::vec2  panOffset       = { 0.f, 0.f };

    Canvas     canvas          = { 600, 400 };
    uint32_t   version         = 0;
    uint32_t   selectedLayerID = 0;
};
