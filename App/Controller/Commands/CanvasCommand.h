#pragma once

#include <Controller/Command.h>
#include <Canvas/Pixel.h>
#include <functional>
#include <vector>

class Canvas;

namespace Cmd {

    class CanvasCommand final : public Command {
    public:
        CanvasCommand(CommandNames name, std::move_only_function<void(Canvas&)> effectFunc);

        void execute(EditorState& state) override;
        void undo(EditorState& state)    override;

    private:
        std::move_only_function<void(Canvas&)> m_effectFunc;
        std::vector<std::vector<Pixel>>        m_backup;
        uint32_t                               m_width  = 0;
        uint32_t                               m_height = 0;
    };

    // -----------------------------------------------------------------------------------------------------------------

    std::unique_ptr<CanvasCommand> addLayer();
    std::unique_ptr<CanvasCommand> deleteLayer(size_t layerID);
    std::unique_ptr<CanvasCommand> duplicateLayer(size_t layerID);

    std::unique_ptr<CanvasCommand> mergeWithLayerBelow(size_t layerID);
    std::unique_ptr<CanvasCommand> mergeAllLayers();

    std::unique_ptr<CanvasCommand> moveLayerToIndex(size_t layerID, size_t index);

    std::unique_ptr<CanvasCommand> rotateLeft();
    std::unique_ptr<CanvasCommand> rotateRight();
    std::unique_ptr<CanvasCommand> rotate180();

    std::unique_ptr<CanvasCommand> flipHorizCanvas();
    std::unique_ptr<CanvasCommand> flipVertCanvas();

    std::unique_ptr<CanvasCommand> resize(uint32_t width, uint32_t height);
    std::unique_ptr<CanvasCommand> scale(uint32_t width, uint32_t height);

    std::unique_ptr<CanvasCommand> seamCarving(uint32_t width, uint32_t height);

}
