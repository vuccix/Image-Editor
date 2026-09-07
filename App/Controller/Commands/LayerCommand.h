#pragma once

#include <Controller/Command.h>
#include <Canvas/Pixel.h>
#include <functional>
#include <vector>

class Layer;

namespace Cmd {

    class LayerCommand final : public Command {
    public:
        using Function = std::move_only_function<void(Layer&)>;

        LayerCommand(CommandNames name, Function execute);

        void execute(EditorState& state) override;
        void undo(EditorState& state)    override;

    private:
        std::move_only_function<void(Layer&)> m_execute;
        std::vector<Pixel>                    m_backup;
        size_t                                m_layerID = SIZE_MAX;
    };

    // -----------------------------------------------------------------------------------------------------------------

    std::unique_ptr<Command> brightness(int32_t value);
    std::unique_ptr<Command> contrast(float factor);
    std::unique_ptr<Command> saturation(float factor);

    std::unique_ptr<Command> grayscale();
    std::unique_ptr<Command> luminance();
    std::unique_ptr<Command> sepia();

    std::unique_ptr<Command> duoTone(const float colorA[3], const float colorB[3]);

    std::unique_ptr<Command> blur(int32_t amount);
    std::unique_ptr<Command> gaussianBlur(int32_t amount);
    std::unique_ptr<Command> motionBlur(int32_t distance, float angle);

    std::unique_ptr<Command> emboss();
    std::unique_ptr<Command> outline();
    std::unique_ptr<Command> sharpen(float amount);
    std::unique_ptr<Command> pixelate(int32_t blockSize);

    std::unique_ptr<Command> canny(float lowThreshold, float highThreshold);
    std::unique_ptr<Command> laplace();
    std::unique_ptr<Command> prewitt();
    std::unique_ptr<Command> scharr();
    std::unique_ptr<Command> sobel();

    std::unique_ptr<Command> normalMap(float strength, bool flipY);

}
