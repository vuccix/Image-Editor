#include "LayerCommand.h"
#include <Processing/Filters.h>
#include <Processing/Effects.h>

namespace Cmd {

LayerCommand::LayerCommand(std::string name, std::move_only_function<void(Layer&)> effectFunc)
    : m_name(std::move(name)), m_effectFunc(std::move(effectFunc)) {}

void LayerCommand::execute(EditorState& state) {
    m_layerID    = state.selectedLayerID;
    Layer& layer = state.canvas[m_layerID];
    m_backup     = layer.copyData();

    m_effectFunc(layer);
    ++state.version;
}

void LayerCommand::undo(EditorState& state) {
    assert(m_backup.empty() == false);

    Layer& layer = state.canvas[m_layerID];
    layer.setData(std::move(m_backup));
    ++state.version;
}

std::string LayerCommand::getName() const {
    return m_name;
}

std::unique_ptr<LayerCommand> flipHoriz() {
    return std::make_unique<LayerCommand>(
        "Flip Horizontally", [](Layer& layer) {
            Effects::flipHorizontally(layer);
        }
    );
}

std::unique_ptr<LayerCommand> flipVert() {
    return std::make_unique<LayerCommand>(
        "Flip Vertically", [](Layer& layer) {
            Effects::flipVertically(layer);
        }
    );
}

std::unique_ptr<LayerCommand> invert() {
    return std::make_unique<LayerCommand>(
        "Invert Colors", [](Layer& layer) {
            Effects::invert(layer);
        }
    );
}

std::unique_ptr<LayerCommand> invertAlpha() {
    return std::make_unique<LayerCommand>(
        "Invert Alpha", [](Layer& layer) {
            Effects::invertAlpha(layer);
        }
    );
}

std::unique_ptr<LayerCommand> brightness(const int32_t value) {
    return std::make_unique<LayerCommand>(
        "Brightness", [value](Layer& layer) {
            Effects::brightness(layer, value);
        }
    );
}

std::unique_ptr<LayerCommand> contrast(const float factor) {
    return std::make_unique<LayerCommand>(
        "Contrast", [factor](Layer& layer) {
            Effects::contrast(layer, factor);
        }
    );
}

std::unique_ptr<LayerCommand> saturation(const float factor) {
    return std::make_unique<LayerCommand>(
        "Saturation", [factor](Layer& layer) {
            Effects::saturation(layer, factor);
        }
    );
}

std::unique_ptr<LayerCommand> grayscale() {
    return std::make_unique<LayerCommand>(
        "Grayscale", [](Layer& layer) {
            Effects::grayscale(layer);
        }
    );
}

std::unique_ptr<LayerCommand> luminance() {
    return std::make_unique<LayerCommand>(
        "Luminance", [](Layer& layer) {
            Effects::luminance(layer);
        }
    );
}

std::unique_ptr<LayerCommand> sepia() {
    return std::make_unique<LayerCommand>(
        "Sepia", [](Layer& layer) {
            Effects::sepia(layer);
        }
    );
}

}
