#pragma once

#include <Controller/Command.h>
#include <functional>
#include <cstdint>

struct EditorState;

namespace Cmd {

    class InvertibleCommand final : public Command {
    public:
        InvertibleCommand(CommandNames name, std::move_only_function<void(EditorState&)> effectFunc,
                                             std::move_only_function<void(EditorState&)> invertFunc);

        void execute(EditorState& state) override;
        void undo(EditorState& state)    override;

    private:
        std::move_only_function<void(EditorState&)> m_effectFunc;
        std::move_only_function<void(EditorState&)> m_invertFunc;
    };

    // -----------------------------------------------------------------------------------------------------------------

    std::unique_ptr<InvertibleCommand> flipLayerHoriz();
    std::unique_ptr<InvertibleCommand> flipLayerVert();

    std::unique_ptr<InvertibleCommand> flipCanvasHoriz();
    std::unique_ptr<InvertibleCommand> flipCanvasVert();

    std::unique_ptr<InvertibleCommand> rotateLeft();
    std::unique_ptr<InvertibleCommand> rotateRight();
    std::unique_ptr<InvertibleCommand> rotate180();

    std::unique_ptr<InvertibleCommand> invert();
    std::unique_ptr<InvertibleCommand> invertAlpha();
    std::unique_ptr<InvertibleCommand> swapChannels(int32_t combination);

    std::unique_ptr<InvertibleCommand> addLayer();
    std::unique_ptr<InvertibleCommand> duplicateLayer(size_t layerID);

    std::unique_ptr<InvertibleCommand> moveLayerToIndex(size_t layerID, size_t index);

    std::unique_ptr<InvertibleCommand> changeOpacity(float oldOpacity, float newOpacity);
    std::unique_ptr<InvertibleCommand> changeFill(float oldFill, float newFill);
    // std::unique_ptr<InvertibleCommand> changeBlendMode();

}
