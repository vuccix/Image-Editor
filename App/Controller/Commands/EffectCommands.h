#pragma once

#include <Controller/Command.h>
#include <functional>
#include <string>
#include <vector>

namespace Cmd {

    class LayerEffectCommand : public Command {
    public:
        LayerEffectCommand(std::string name, std::move_only_function<void(Layer&)> effectFunc);

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

    std::unique_ptr<LayerEffectCommand> flipHoriz();
    std::unique_ptr<LayerEffectCommand> flipVert();
    std::unique_ptr<LayerEffectCommand> invert();
    std::unique_ptr<LayerEffectCommand> invertAlpha();

}
