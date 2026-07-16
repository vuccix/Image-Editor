#include "CanvasCommand.h"
#include <Processing/Filters.h>
#include <Processing/Effects.h>
#include <iostream>
#include <chrono>

Cmd::CanvasCommand::CanvasCommand(const CommandNames name, std::move_only_function<void(Canvas&)> effectFunc)
    : Command(name), m_effectFunc(std::move(effectFunc)) {}

void Cmd::CanvasCommand::execute(EditorState& state) {
    using clock = std::chrono::steady_clock;
    const auto start = clock::now();

    // --------------------------------------------------------------
    Canvas& canvas = state.canvas;
    m_width        = canvas.width();
    m_height       = canvas.height();

    m_backup.reserve(canvas.layerCount());
    for (const Layer& l : canvas)
        m_backup.emplace_back(l.data().begin(), l.data().end());

    m_effectFunc(canvas);
    ++state.version;
    // --------------------------------------------------------------

    const auto end = clock::now();
    const auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << getName() << " took " << dur.count() << " ms\n";
}

void Cmd::CanvasCommand::undo(EditorState& state) {
    assert(m_backup.empty() == false);
    assert(m_width > 0 && m_height > 0);

    Canvas& canvas = state.canvas;
    canvas.resize(m_width, m_height);

    for (size_t i = 0; i < canvas.layerCount(); ++i)
        canvas[i].setData(std::move(m_backup[i]));

    ++state.version;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Cmd {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

std::unique_ptr<CanvasCommand> rotateLeft() {
    return std::make_unique<CanvasCommand>(
        CommandNames::RotateLeft, [](Canvas& canvas) {
            Effects::rotateLeft(canvas);
        }
    );
}

std::unique_ptr<CanvasCommand> rotateRight() {
    return std::make_unique<CanvasCommand>(
        CommandNames::RotateRight, [](Canvas& canvas) {
            Effects::rotateRight(canvas);
        }
    );
}

std::unique_ptr<CanvasCommand> rotate180() {
    return std::make_unique<CanvasCommand>(
        CommandNames::Rotate180, [](Canvas& canvas) {
            Effects::rotate180(canvas);
        }
    );
}

std::unique_ptr<CanvasCommand> flipHorizCanvas() {
    return std::make_unique<CanvasCommand>(
        CommandNames::FlipHorizontally, [](Canvas& canvas) {
            Effects::flipHorizontally(canvas);
        }
    );
}

std::unique_ptr<CanvasCommand> flipVertCanvas() {
    return std::make_unique<CanvasCommand>(
        CommandNames::FlipVertically, [](Canvas& canvas) {
            Effects::flipVertically(canvas);
        }
    );
}

std::unique_ptr<CanvasCommand> resize(const uint32_t width, const uint32_t height) {
    return std::make_unique<CanvasCommand>(
        CommandNames::Resize, [width, height](Canvas& canvas) {
            canvas.resize(width, height);
        }
    );
}

std::unique_ptr<CanvasCommand> scale(const uint32_t width, const uint32_t height) {
    return std::make_unique<CanvasCommand>(
        CommandNames::CanvasSize, [width, height](Canvas& canvas) {
            canvas.scale(width, height);
        }
    );
}

std::unique_ptr<CanvasCommand> seamCarving(uint32_t width, uint32_t height) {
    return std::make_unique<CanvasCommand>(
        CommandNames::SeamCarving, [width, height](Canvas& canvas) {
            Filters::seamCarving(canvas, width, height);
        }
    );
}

#pragma GCC diagnostic pop

}
