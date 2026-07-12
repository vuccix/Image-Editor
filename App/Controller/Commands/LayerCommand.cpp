#include "LayerCommand.h"
#include <Processing/Filters.h>
#include <Processing/Effects.h>
#include <iostream>
#include <chrono>

namespace Cmd {

LayerCommand::LayerCommand(std::string name, std::move_only_function<void(Layer&)> effectFunc)
    : m_name(std::move(name)), m_effectFunc(std::move(effectFunc)) {}

void LayerCommand::execute(EditorState& state) {
    using clock = std::chrono::steady_clock;
    const auto start = clock::now();

    // --------------------------------------------------------------
    m_layerID    = state.selectedLayerID;
    Layer& layer = state.canvas[m_layerID];
    m_backup     = layer.copyData();

    m_effectFunc(layer);
    ++state.version;
    // --------------------------------------------------------------

    const auto end = clock::now();
    const auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << m_name << " took " << dur.count() << " ms\n";
}

void LayerCommand::undo(EditorState& state) {
    assert(m_backup.empty() == false);
    assert(m_layerID != SIZE_MAX);

    Layer& layer = state.canvas[m_layerID];
    layer.setData(std::move(m_backup));
    ++state.version;
}

std::string LayerCommand::getName() const {
    return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

std::unique_ptr<LayerCommand> blur(const int32_t amount) {
    return std::make_unique<LayerCommand>(
        "Blur", [amount](Layer& layer) {
            Filters::blur(layer, amount);
        }
    );
}

std::unique_ptr<LayerCommand> swapChannels(const int32_t combination) {
    return std::make_unique<LayerCommand>(
        "Swap Channels", [combination](Layer& layer) {
            Filters::swapChannels(layer, combination);
        }
    );
}

std::unique_ptr<LayerCommand> emboss() {
    return std::make_unique<LayerCommand>(
        "Emboss", [](Layer& layer) {
            Filters::emboss(layer);
        }
    );
}

std::unique_ptr<LayerCommand> outline() {
    return std::make_unique<LayerCommand>(
        "Outline", [](Layer& layer) {
            Filters::outline(layer);
        }
    );
}

std::unique_ptr<LayerCommand> sharpen(const float amount) {
    return std::make_unique<LayerCommand>(
        "Sharpen", [amount](Layer& layer) {
            Filters::sharpen(layer, amount);
        }
    );
}

std::unique_ptr<LayerCommand> pixelate(const int blockSize) {
    return std::make_unique<LayerCommand>(
        "Pixelate", [blockSize](Layer& layer) {
            Filters::pixelate(layer, blockSize);
        }
    );
}

std::unique_ptr<LayerCommand> laplace() {
    return std::make_unique<LayerCommand>(
        "Laplace", [](Layer& layer) {
            Filters::laplace(layer);
        }
    );
}

std::unique_ptr<LayerCommand> prewitt() {
    return std::make_unique<LayerCommand>(
        "Prewitt", [](Layer& layer) {
            Filters::prewitt(layer);
        }
    );
}

std::unique_ptr<LayerCommand> scharr() {
    return std::make_unique<LayerCommand>(
        "Scharr", [](Layer& layer) {
            Filters::scharr(layer);
        }
    );
}

std::unique_ptr<LayerCommand> sobel() {
    return std::make_unique<LayerCommand>(
        "Sobel", [](Layer& layer) {
            Filters::sobel(layer);
        }
    );
}

}
