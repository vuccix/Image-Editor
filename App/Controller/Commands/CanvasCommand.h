#pragma once

#include <Controller/Command.h>
#include <Canvas/Canvas.h>
#include <functional>
#include <vector>

namespace Cmd {

    class CanvasCommand final : public Command {
    public:
        CanvasCommand(CommandNames name, std::move_only_function<void(Canvas&)> effectFunc);

        void execute(EditorState& state) override;
        void undo(EditorState& state)    override;

    private:
        std::move_only_function<void(Canvas&)> m_effectFunc;
        std::vector<Layer>                     m_backup;
        uint32_t                               m_width  = 0;
        uint32_t                               m_height = 0;
    };

    // -----------------------------------------------------------------------------------------------------------------

    std::unique_ptr<Command> mergeAllLayers();

    std::unique_ptr<Command> resize(uint32_t width, uint32_t height);
    std::unique_ptr<Command> scale(uint32_t width, uint32_t height);

    std::unique_ptr<Command> seamCarving(uint32_t width, uint32_t height);

}
