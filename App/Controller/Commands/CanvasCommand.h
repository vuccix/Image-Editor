#pragma once

#include <Controller/Command.h>
#include <functional>
#include <string>
#include <vector>

namespace Cmd {

    class CanvasCommand final : public Command {
    public:
        CanvasCommand(std::string name, std::move_only_function<void(Canvas&)> effectFunc);

        void execute(EditorState& state) override;
        void undo(EditorState& state)    override;
        std::string getName() const      override;

    private:
        std::string                            m_name;
        std::move_only_function<void(Canvas&)> m_effectFunc;
        std::vector<std::vector<Pixel>>        m_backup;
        uint32_t                               m_width  = 0;
        uint32_t                               m_height = 0;
    };

    // -----------------------------------------------------------------------------------------------------------------

    std::unique_ptr<CanvasCommand> rotateLeft();
    std::unique_ptr<CanvasCommand> rotateRight();
    std::unique_ptr<CanvasCommand> rotate180();

    std::unique_ptr<CanvasCommand> flipHorizCanvas();
    std::unique_ptr<CanvasCommand> flipVertCanvas();

    std::unique_ptr<CanvasCommand> resize(uint32_t width, uint32_t height);
    std::unique_ptr<CanvasCommand> scale(uint32_t width, uint32_t height);

    std::unique_ptr<CanvasCommand> seamCarving(uint32_t width, uint32_t height);
}
