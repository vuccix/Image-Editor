#include "Names.h"
#include <cassert>

CommandToString::CommandToString() {
    // Filters
    m_commands[CommandNames::DuoTone]          = "Duo Tone";
    m_commands[CommandNames::WaterColor]       = "Water Color";
    m_commands[CommandNames::OilPainting]      = "Oil Painting";
    m_commands[CommandNames::Blur]             = "Blur";
    m_commands[CommandNames::GaussianBlur]     = "Gaussian Blur";
    m_commands[CommandNames::MotionBlur]       = "Motion Blur";
    m_commands[CommandNames::SwapChannels]     = "Swap Channels";
    m_commands[CommandNames::Emboss]           = "Emboss";
    m_commands[CommandNames::Outline]          = "Outline";
    m_commands[CommandNames::Sharpen]          = "Sharpen";
    m_commands[CommandNames::Pixelate]         = "Pixelate";
    m_commands[CommandNames::Canny]            = "Canny";
    m_commands[CommandNames::Laplace]          = "Laplace";
    m_commands[CommandNames::Prewitt]          = "Prewitt";
    m_commands[CommandNames::Scharr]           = "Scharr";
    m_commands[CommandNames::Sobel]            = "Sobel";
    m_commands[CommandNames::FourierTransform] = "Fourier Transform";
    m_commands[CommandNames::NormalMap]        = "Normal Map";
    m_commands[CommandNames::SeamCarving]      = "Seam Carving";

    // Effects
    m_commands[CommandNames::FlipHorizontally] = "Flip Horizontally";
    m_commands[CommandNames::FlipVertically]   = "Flip Vertically";
    m_commands[CommandNames::RotateLeft]       = "Rotate 90° Left";
    m_commands[CommandNames::RotateRight]      = "Rotate 90° Right";
    m_commands[CommandNames::Rotate180]        = "Rotate 180°";
    m_commands[CommandNames::Brightness]       = "Brightness";
    m_commands[CommandNames::Contrast]         = "Contrast";
    m_commands[CommandNames::Saturation]       = "Saturation";
    m_commands[CommandNames::InvertColors]     = "Invert Colors";
    m_commands[CommandNames::InvertAlpha]      = "Invert Alpha";
    m_commands[CommandNames::Grayscale]        = "Grayscale";
    m_commands[CommandNames::Luminance]        = "Luminance";
    m_commands[CommandNames::Sepia]            = "Sepia";

    // Canvas
    m_commands[CommandNames::Resize]           = "Resize";
    m_commands[CommandNames::CanvasSize]       = "Canvas Size";
    m_commands[CommandNames::AddLayer]         = "Add New Layer";
    m_commands[CommandNames::DeleteLayer]      = "Delete Layer";
    m_commands[CommandNames::DuplicateLayer]   = "Duplicate Layer";
    m_commands[CommandNames::MergeLayer]       = "Merge Layers";
    m_commands[CommandNames::Flatten]          = "Flatten Image";
    m_commands[CommandNames::Reorder]          = "Change Layer Order";
    m_commands[CommandNames::Crop]             = "Crop to Selection";

    // Layer
    m_commands[CommandNames::ToggleVisibility] = "Toggle Layer Visibility";
    m_commands[CommandNames::ChangeOpacity]    = "Change Opacity";
    m_commands[CommandNames::ChangeFill]       = "Change Fill";
    m_commands[CommandNames::ChangeBlendMode]  = "Change Blend Mode";
}

void CommandToString::add(const CommandNames name, std::string value) {
    assert(name                   != CommandNames::Count);
    assert(value.empty()          == false);
    assert(m_commands.count(name) == false);

    m_commands[name] = std::move(value);
}

std::string_view CommandToString::toString(const CommandNames name) {
    assert(name != CommandNames::Count);
    assert(m_commands.contains(name));

    return m_commands[name];
}

CommandToString CmdToString;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Controller/Command.h>

Command::Command(const CommandNames name) : m_name(name) {}

std::string_view Command::getName() const {
    return CmdToString.toString(m_name);
}
