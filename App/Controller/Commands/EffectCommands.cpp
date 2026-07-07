#include "EffectCommands.h"
#include <Processing/Effects.h>

namespace Cmd {

LayerEffectCommand::LayerEffectCommand(std::string name, std::move_only_function<void(Layer&)> effectFunc)
    : m_name(std::move(name)), m_effectFunc(std::move(effectFunc)) {}

void LayerEffectCommand::execute(EditorState& state) {
    m_layerID    = state.selectedLayerID;
    Layer& layer = state.canvas[m_layerID];
    m_backup     = layer.copyData();

    m_effectFunc(layer);
    ++state.version;
}

void LayerEffectCommand::undo(EditorState& state) {
    Layer& layer = state.canvas[m_layerID];
    layer.setData(std::move(m_backup));
    ++state.version;
}

std::string LayerEffectCommand::getName() const {
    return m_name;
}

std::unique_ptr<LayerEffectCommand> flipHoriz() {
    return std::make_unique<LayerEffectCommand>(
        "Flip Horizontally", [](Layer& layer) {
            Effects::flipHorizontally(layer);
        }
    );
}

std::unique_ptr<LayerEffectCommand> flipVert() {
    return std::make_unique<LayerEffectCommand>(
        "Flip Vertically", [](Layer& layer) {
            Effects::flipVertically(layer);
        }
    );
}

std::unique_ptr<LayerEffectCommand> invert() {
    return std::make_unique<LayerEffectCommand>(
        "Invert Colors", [](Layer& layer) {
            Effects::invert(layer);
        }
    );
}

std::unique_ptr<LayerEffectCommand> invertAlpha() {
    return std::make_unique<LayerEffectCommand>(
        "Invert Alpha", [](Layer& layer) {
            Effects::invertAlpha(layer);
        }
    );
}

}
