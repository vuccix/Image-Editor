#include "EffectCommands.h"
#include <Processing/Effects.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FlipHorizontalCommand::execute(EditorState& state) {
    m_layerID    = state.selectedLayerID;
    Layer& layer = state.canvas[m_layerID];
    m_backup     = layer.copyData();

    Effects::flipHorizontally(layer);
    ++state.version;
}

void FlipHorizontalCommand::undo(EditorState& state) {
    Layer& layer = state.canvas[m_layerID];
    layer.setData(std::move(m_backup));
    ++state.version;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FlipVerticalCommand::execute(EditorState& state) {
    m_layerID    = state.selectedLayerID;
    Layer& layer = state.canvas[m_layerID];
    m_backup     = layer.copyData();

    Effects::flipVertically(layer);
    ++state.version;
}

void FlipVerticalCommand::undo(EditorState& state) {
    Layer& layer = state.canvas[m_layerID];
    layer.setData(std::move(m_backup));
    ++state.version;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InvertCommand::execute(EditorState& state) {
    m_layerID    = state.selectedLayerID;
    Layer& layer = state.canvas[m_layerID];
    m_backup     = layer.copyData();

    Effects::invert(layer);
    ++state.version;
}

void InvertCommand::undo(EditorState& state) {
    Layer& layer = state.canvas[m_layerID];
    layer.setData(std::move(m_backup));
    ++state.version;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InvertAlphaCommand::execute(EditorState& state) {
    m_layerID    = state.selectedLayerID;
    Layer& layer = state.canvas[m_layerID];
    m_backup     = layer.copyData();

    Effects::invertAlpha(layer);
    ++state.version;
}

void InvertAlphaCommand::undo(EditorState& state) {
    Layer& layer = state.canvas[m_layerID];
    layer.setData(std::move(m_backup));
    ++state.version;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
