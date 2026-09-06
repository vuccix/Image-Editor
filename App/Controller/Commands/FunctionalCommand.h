#pragma once

#include <Controller/Command.h>
#include <functional>
#include <cstdint>

namespace Cmd {

    class FunctionalCommand final : public Command {
    public:
        FunctionalCommand(CommandNames name, std::move_only_function<void(EditorState&)> effectFunc,
                                             std::move_only_function<void(EditorState&)> invertFunc);

        void execute(EditorState& state) override;
        void undo(EditorState& state)    override;

    private:
        std::move_only_function<void(EditorState&)> m_effectFunc;
        std::move_only_function<void(EditorState&)> m_invertFunc;
    };

    // -----------------------------------------------------------------------------------------------------------------

    std::unique_ptr<FunctionalCommand> flipLayerHoriz();
    std::unique_ptr<FunctionalCommand> flipLayerVert();

    std::unique_ptr<FunctionalCommand> flipCanvasHoriz();
    std::unique_ptr<FunctionalCommand> flipCanvasVert();

    std::unique_ptr<FunctionalCommand> rotateLeft();
    std::unique_ptr<FunctionalCommand> rotateRight();
    std::unique_ptr<FunctionalCommand> rotate180();

    std::unique_ptr<FunctionalCommand> invert();
    std::unique_ptr<FunctionalCommand> invertAlpha();
    std::unique_ptr<FunctionalCommand> swapChannels(int32_t combination);

    std::unique_ptr<FunctionalCommand> addLayer();
    std::unique_ptr<FunctionalCommand> duplicateLayer(size_t layerID);

    std::unique_ptr<FunctionalCommand> moveLayerToIndex(size_t layerID, size_t index);

    std::unique_ptr<FunctionalCommand> changeOpacity(float oldOpacity, float newOpacity);
    std::unique_ptr<FunctionalCommand> changeFill(float oldFill, float newFill);
    // std::unique_ptr<FunctionalCommand> changeBlendMode();

}
