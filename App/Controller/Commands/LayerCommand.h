#pragma once

#include <Controller/Command.h>
#include <functional>
#include <string>
#include <vector>

namespace Cmd {

    class LayerCommand : public Command {
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

    std::unique_ptr<LayerCommand> blur();

    std::unique_ptr<LayerCommand> swapChannels(int32_t combination);
    std::unique_ptr<LayerCommand> emboss();
    std::unique_ptr<LayerCommand> outline();

}
