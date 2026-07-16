#include "LayerCommand.h"
#include <Processing/Filters.h>
#include <Processing/Effects.h>
#include <iostream>
#include <chrono>

Cmd::LayerCommand::LayerCommand(const CommandNames name, std::move_only_function<void(Layer&)> effectFunc)
    : Command(name), m_effectFunc(std::move(effectFunc)) {}

void Cmd::LayerCommand::execute(EditorState& state) {
    using clock = std::chrono::steady_clock;
    const auto start = clock::now();

    // --------------------------------------------------------------
    m_layerID    = state.selectedLayerID;
    Layer& layer = state.canvas[m_layerID];
    m_backup     = std::vector(layer.data().begin(), layer.data().end());

    m_effectFunc(layer);
    ++state.version;
    // --------------------------------------------------------------

    const auto end = clock::now();
    const auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << getName() << " took " << dur.count() << " ms\n";
}

void Cmd::LayerCommand::undo(EditorState& state) {
    assert(m_backup.empty() == false);
    assert(m_layerID != SIZE_MAX);

    Layer& layer = state.canvas[m_layerID];
    layer.setData(std::move(m_backup));
    ++state.version;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Cmd {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

std::unique_ptr<LayerCommand> flipHoriz() {
    return std::make_unique<LayerCommand>(
        CommandNames::FlipHorizontally, [](Layer& layer) {
            Effects::flipHorizontally(layer);
        }
    );
}

std::unique_ptr<LayerCommand> flipVert() {
    return std::make_unique<LayerCommand>(
        CommandNames::FlipVertically, [](Layer& layer) {
            Effects::flipVertically(layer);
        }
    );
}

std::unique_ptr<LayerCommand> invert() {
    return std::make_unique<LayerCommand>(
        CommandNames::InvertColors, [](Layer& layer) {
            Effects::invert(layer);
        }
    );
}

std::unique_ptr<LayerCommand> invertAlpha() {
    return std::make_unique<LayerCommand>(
        CommandNames::InvertAlpha, [](Layer& layer) {
            Effects::invertAlpha(layer);
        }
    );
}

std::unique_ptr<LayerCommand> brightness(const int32_t value) {
    return std::make_unique<LayerCommand>(
        CommandNames::Brightness, [value](Layer& layer) {
            Effects::brightness(layer, value);
        }
    );
}

std::unique_ptr<LayerCommand> contrast(const float factor) {
    return std::make_unique<LayerCommand>(
        CommandNames::Contrast, [factor](Layer& layer) {
            Effects::contrast(layer, factor);
        }
    );
}

std::unique_ptr<LayerCommand> saturation(const float factor) {
    return std::make_unique<LayerCommand>(
        CommandNames::Saturation, [factor](Layer& layer) {
            Effects::saturation(layer, factor);
        }
    );
}

std::unique_ptr<LayerCommand> grayscale() {
    return std::make_unique<LayerCommand>(
        CommandNames::Grayscale, [](Layer& layer) {
            Effects::grayscale(layer);
        }
    );
}

std::unique_ptr<LayerCommand> luminance() {
    return std::make_unique<LayerCommand>(
        CommandNames::Luminance, [](Layer& layer) {
            Effects::luminance(layer);
        }
    );
}

std::unique_ptr<LayerCommand> sepia() {
    return std::make_unique<LayerCommand>(
        CommandNames::Sepia, [](Layer& layer) {
            Effects::sepia(layer);
        }
    );
}

std::unique_ptr<LayerCommand> duoTone(const float colorA[3], const float colorB[3]) {
    return std::make_unique<LayerCommand>(
        CommandNames::DuoTone, [colorA, colorB](Layer& layer) {
            Filters::duoTone(layer, colorA, colorB);
        }
    );
}

std::unique_ptr<LayerCommand> blur(const int32_t amount) {
    return std::make_unique<LayerCommand>(
        CommandNames::Blur, [amount](Layer& layer) {
            Filters::blur(layer, amount);
        }
    );
}

std::unique_ptr<LayerCommand> gaussianBlur(const int32_t amount) {
    return std::make_unique<LayerCommand>(
        CommandNames::GaussianBlur, [amount](Layer& layer) {
            Filters::gaussianBlur(layer, amount);
        }
    );
}

std::unique_ptr<LayerCommand> motionBlur(const int32_t distance, const float angle) {
    return std::make_unique<LayerCommand>(
        CommandNames::MotionBlur, [distance, angle](Layer& layer) {
            Filters::motionBlur(layer, distance, angle);
        }
    );
}

std::unique_ptr<LayerCommand> swapChannels(const int32_t combination) {
    return std::make_unique<LayerCommand>(
        CommandNames::SwapChannels, [combination](Layer& layer) {
            Filters::swapChannels(layer, combination);
        }
    );
}

std::unique_ptr<LayerCommand> emboss() {
    return std::make_unique<LayerCommand>(
        CommandNames::Emboss, [](Layer& layer) {
            Filters::emboss(layer);
        }
    );
}

std::unique_ptr<LayerCommand> outline() {
    return std::make_unique<LayerCommand>(
        CommandNames::Outline, [](Layer& layer) {
            Filters::outline(layer);
        }
    );
}

std::unique_ptr<LayerCommand> sharpen(const float amount) {
    return std::make_unique<LayerCommand>(
        CommandNames::Sharpen, [amount](Layer& layer) {
            Filters::sharpen(layer, amount);
        }
    );
}

std::unique_ptr<LayerCommand> pixelate(const int blockSize) {
    return std::make_unique<LayerCommand>(
        CommandNames::Pixelate, [blockSize](Layer& layer) {
            Filters::pixelate(layer, blockSize);
        }
    );
}

std::unique_ptr<LayerCommand> canny(const float lowThreshold, const float highThreshold) {
    return std::make_unique<LayerCommand>(
        CommandNames::Canny, [lowThreshold, highThreshold](Layer& layer) {
            Filters::canny(layer, lowThreshold, highThreshold);
        }
    );
}

std::unique_ptr<LayerCommand> laplace() {
    return std::make_unique<LayerCommand>(
        CommandNames::Laplace, [](Layer& layer) {
            Filters::laplace(layer);
        }
    );
}

std::unique_ptr<LayerCommand> prewitt() {
    return std::make_unique<LayerCommand>(
        CommandNames::Prewitt, [](Layer& layer) {
            Filters::prewitt(layer);
        }
    );
}

std::unique_ptr<LayerCommand> scharr() {
    return std::make_unique<LayerCommand>(
        CommandNames::Scharr, [](Layer& layer) {
            Filters::scharr(layer);
        }
    );
}

std::unique_ptr<LayerCommand> sobel() {
    return std::make_unique<LayerCommand>(
        CommandNames::Sobel, [](Layer& layer) {
            Filters::sobel(layer);
        }
    );
}

std::unique_ptr<LayerCommand> normalMap(const float strength, const bool flipY) {
    return std::make_unique<LayerCommand>(
        CommandNames::NormalMap, [strength, flipY](Layer& layer) {
            Filters::normalMap(layer, strength, flipY);
        }
    );
}

#pragma GCC diagnostic pop

}
