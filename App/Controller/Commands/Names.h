#pragma once

#include <unordered_map>
#include <string>

enum class CommandNames {
    // Filters
    DuoTone, WaterColor, OilPainting,
    Blur, GaussianBlur, MotionBlur,
    SwapChannels, Emboss, Outline, Sharpen, Pixelate,
    Canny, Laplace, Prewitt, Scharr, Sobel,
    FourierTransform, NormalMap, SeamCarving,

    // Effects
    FlipHorizontally, FlipVertically,
    RotateLeft, RotateRight, Rotate180,
    Brightness, Contrast, Saturation,
    InvertColors, InvertAlpha, Grayscale, Luminance, Sepia,

    // Canvas
    Resize, CanvasSize,
    AddLayer, DeleteLayer, DuplicateLayer,
    MergeLayer, Flatten, Reorder,
    Crop,

    // Layer
    ToggleVisibility,
    ChangeOpacity, ChangeFill, ChangeBlendMode,

    Count
};

class CommandToString {
public:
    CommandToString();

    void add(CommandNames name, std::string value);
    std::string_view toString(CommandNames name);

    // TODO: void switchLang(Language lang);

private:
    std::unordered_map<CommandNames, std::string> m_commands;
};

extern CommandToString CmdToString;
