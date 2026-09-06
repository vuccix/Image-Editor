#include "LayerCommand.h"
#include <Model/EditorState.h>
#include <Processing/Filters.h>
#include <Processing/Effects.h>
#include <cstdint>

Cmd::LayerCommand::LayerCommand(const CommandNames name, std::move_only_function<void(Layer&)> effectFunc)
    : Command(name), m_effectFunc(std::move(effectFunc)) {}

void Cmd::LayerCommand::execute(EditorState& state) {
    m_layerID    = state.selectedLayerID;
    Layer& layer = state.canvas[m_layerID];
    m_backup     = std::vector(layer.data().begin(), layer.data().end());

    m_effectFunc(layer);
}

void Cmd::LayerCommand::undo(EditorState& state) {
    assert(m_backup.empty() == false);
    assert(m_layerID != SIZE_MAX);

    Layer& layer = state.canvas[m_layerID];
    layer.setData(std::move(m_backup));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Cmd {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

template <typename F, typename... Args>
auto makeCmd(CommandNames name, F&& f, Args&&... args) {
    return std::make_unique<LayerCommand>(
        name, [f = std::forward<F>(f), ...args = std::forward<Args>(args)](Layer& layer) mutable {
            std::invoke(f, layer, args...);
        }
    );
}

#define DEFINE_COMMAND(name, func, ...) \
    return makeCmd(CommandNames::name, func __VA_OPT__(,) __VA_ARGS__)

std::unique_ptr<Command> brightness(const int32_t value) {
    DEFINE_COMMAND(Brightness, Effects::brightness, value);
}

std::unique_ptr<Command> contrast(const float factor) {
    DEFINE_COMMAND(Contrast, Effects::contrast, factor);
}

std::unique_ptr<Command> saturation(const float factor) {
    DEFINE_COMMAND(Saturation, Effects::saturation, factor);
}

std::unique_ptr<Command> grayscale() {
    DEFINE_COMMAND(Grayscale, Effects::grayscale);
}

std::unique_ptr<Command> luminance() {
    DEFINE_COMMAND(Luminance, Effects::luminance);
}

std::unique_ptr<Command> sepia() {
    DEFINE_COMMAND(Sepia, Effects::sepia);
}

std::unique_ptr<Command> duoTone(const float colorA[3], const float colorB[3]) {
    DEFINE_COMMAND(DuoTone, Filters::duoTone, colorA, colorB);
}

std::unique_ptr<Command> blur(const int32_t amount) {
    DEFINE_COMMAND(Blur, Filters::blur, amount);
}

std::unique_ptr<Command> gaussianBlur(const int32_t amount) {
    DEFINE_COMMAND(GaussianBlur, Filters::gaussianBlur, amount);
}

std::unique_ptr<Command> motionBlur(const int32_t distance, const float angle) {
    DEFINE_COMMAND(MotionBlur, Filters::motionBlur, distance, angle);
}

std::unique_ptr<Command> emboss() {
    DEFINE_COMMAND(Emboss, Filters::emboss);
}

std::unique_ptr<Command> outline() {
    DEFINE_COMMAND(Outline, Filters::outline);
}

std::unique_ptr<Command> sharpen(const float amount) {
    DEFINE_COMMAND(Sharpen, Filters::sharpen, amount);
}

std::unique_ptr<Command> pixelate(const int32_t blockSize) {
    DEFINE_COMMAND(Pixelate, Filters::pixelate, blockSize);
}

std::unique_ptr<Command> canny(const float lowThreshold, const float highThreshold) {
    DEFINE_COMMAND(Canny, Filters::canny, lowThreshold, highThreshold);
}

std::unique_ptr<Command> laplace() {
    DEFINE_COMMAND(Laplace, Filters::laplace);
}

std::unique_ptr<Command> prewitt() {
    DEFINE_COMMAND(Prewitt, Filters::prewitt);
}

std::unique_ptr<Command> scharr() {
    DEFINE_COMMAND(Scharr, Filters::scharr);
}

std::unique_ptr<Command> sobel() {
    DEFINE_COMMAND(Sobel, Filters::sobel);
}

std::unique_ptr<Command> normalMap(const float strength, const bool flipY) {
    DEFINE_COMMAND(NormalMap, Filters::normalMap, strength, flipY);
}

#undef DEFINE_COMMAND
#pragma GCC diagnostic pop

}
