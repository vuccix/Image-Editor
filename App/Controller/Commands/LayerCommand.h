#pragma once

#include <Controller/Command.h>
#include <functional>
#include <string>
#include <vector>

namespace Cmd {

    class LayerCommand final : public Command {
    public:
        LayerCommand(std::string name, std::move_only_function<void(Layer&)> effectFunc);

        void execute(EditorState& state) override;
        void undo(EditorState& state)    override;
        std::string getName() const      override;

    private:
        std::string                           m_name;
        std::move_only_function<void(Layer&)> m_effectFunc;
        std::vector<Pixel>                    m_backup;
        size_t                                m_layerID = SIZE_MAX;
    };

    // -----------------------------------------------------------------------------------------------------------------

    std::unique_ptr<LayerCommand> flipHoriz();
    std::unique_ptr<LayerCommand> flipVert();

    std::unique_ptr<LayerCommand> invert();
    std::unique_ptr<LayerCommand> invertAlpha();

    std::unique_ptr<LayerCommand> brightness(int32_t value);
    std::unique_ptr<LayerCommand> contrast(float factor);
    std::unique_ptr<LayerCommand> saturation(float factor);

    std::unique_ptr<LayerCommand> grayscale();
    std::unique_ptr<LayerCommand> luminance();
    std::unique_ptr<LayerCommand> sepia();

    std::unique_ptr<LayerCommand> duoTone(const float colorA[3], const float colorB[3]);

    std::unique_ptr<LayerCommand> blur(int32_t amount);
    std::unique_ptr<LayerCommand> gaussianBlur(int32_t amount);
    std::unique_ptr<LayerCommand> motionBlur(int32_t distance, float angle);

    std::unique_ptr<LayerCommand> swapChannels(int32_t combination);
    std::unique_ptr<LayerCommand> emboss();
    std::unique_ptr<LayerCommand> outline();
    std::unique_ptr<LayerCommand> sharpen(float amount);
    std::unique_ptr<LayerCommand> pixelate(int blockSize);

    std::unique_ptr<LayerCommand> canny(float lowThreshold, float highThreshold);
    std::unique_ptr<LayerCommand> laplace();
    std::unique_ptr<LayerCommand> prewitt();
    std::unique_ptr<LayerCommand> scharr();
    std::unique_ptr<LayerCommand> sobel();

    std::unique_ptr<LayerCommand> normalMap(float strength, bool flipY);

}
