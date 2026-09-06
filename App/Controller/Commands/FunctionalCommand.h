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

    std::unique_ptr<Command> flipLayerHoriz();
    std::unique_ptr<Command> flipLayerVert();

    std::unique_ptr<Command> flipCanvasHoriz();
    std::unique_ptr<Command> flipCanvasVert();

    std::unique_ptr<Command> rotateLeft();
    std::unique_ptr<Command> rotateRight();
    std::unique_ptr<Command> rotate180();

    std::unique_ptr<Command> invert();
    std::unique_ptr<Command> invertAlpha();
    std::unique_ptr<Command> swapChannels(int32_t combination);

    std::unique_ptr<Command> addLayer();
    std::unique_ptr<Command> duplicateLayer(size_t layerID);

    std::unique_ptr<Command> moveLayerToIndex(size_t layerID, size_t index);

    std::unique_ptr<Command> changeOpacity(float oldOpacity, float newOpacity);
    std::unique_ptr<Command> changeFill(float oldFill, float newFill);
    // std::unique_ptr<Command> changeBlendMode();

}
