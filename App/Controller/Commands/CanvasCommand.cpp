#include "CanvasCommand.h"
#include <Processing/Filters.h>
#include <Processing/Effects.h>

namespace Cmd {

CanvasCommand::CanvasCommand(std::string name, std::move_only_function<void(Canvas&)> effectFunc)
    : m_name(std::move(name)), m_effectFunc(std::move(effectFunc)) {}

void CanvasCommand::execute(EditorState& state) {
    Canvas& canvas = state.canvas;
    m_width        = canvas.width();
    m_height       = canvas.height();

    m_backup.reserve(canvas.layerCount());
    for (size_t i = 0; i < canvas.layerCount(); ++i)
        m_backup.emplace_back(canvas[i].copyData());

    m_effectFunc(canvas);
    ++state.version;
}

void CanvasCommand::undo(EditorState& state) {
    assert(m_backup.empty() == false);
    assert(m_width > 0 && m_height > 0);

    Canvas& canvas = state.canvas;
    canvas.resize(m_width, m_height);

    for (size_t i = 0; i < canvas.layerCount(); ++i)
        canvas[i].setData(std::move(m_backup[i]));

    ++state.version;
}

std::string CanvasCommand::getName() const {
    return m_name;
}

std::unique_ptr<CanvasCommand> rotateLeft() {
    return std::make_unique<CanvasCommand>(
        "Rotate 90° Left", [](Canvas& canvas) {
            Effects::rotateLeft(canvas);
        }
    );
}

std::unique_ptr<CanvasCommand> rotateRight() {
    return std::make_unique<CanvasCommand>(
        "Rotate 90° Right", [](Canvas& canvas) {
            Effects::rotateRight(canvas);
        }
    );
}

std::unique_ptr<CanvasCommand> rotate180() {
    return std::make_unique<CanvasCommand>(
        "Rotate 180°", [](Canvas& canvas) {
            Effects::rotate180(canvas);
        }
    );
}

std::unique_ptr<CanvasCommand> flipHorizCanvas() {
    return std::make_unique<CanvasCommand>(
        "Flip Horizontally", [](Canvas& canvas) {
            Effects::flipHorizontally(canvas);
        }
    );
}

std::unique_ptr<CanvasCommand> flipVertCanvas() {
    return std::make_unique<CanvasCommand>(
        "Flip Vertically", [](Canvas& canvas) {
            Effects::flipVertically(canvas);
        }
    );
}

std::unique_ptr<CanvasCommand> resize(const uint32_t width, const uint32_t height) {
    return std::make_unique<CanvasCommand>(
        "Resize", [width, height](Canvas& canvas) {
            canvas.resize(width, height);
        }
    );
}

std::unique_ptr<CanvasCommand> scale(const uint32_t width, const uint32_t height) {
    return std::make_unique<CanvasCommand>(
        "Scale", [width, height](Canvas& canvas) {
            canvas.scale(width, height);
        }
    );
}

}
