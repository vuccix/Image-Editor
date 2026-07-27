#include "CanvasCommand.h"
#include <Model/EditorState.h>
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

template <typename F, typename... Args>
auto makeCmd(CommandNames name, F&& f, Args&&... args) {
    return std::make_unique<CanvasCommand>(
        name, [f = std::forward<F>(f), ...args = std::forward<Args>(args)](Canvas& canvas) mutable {
            std::invoke(f, canvas, args...);
        }
    );
}

#define DEFINE_COMMAND(name, func, ...) \
    return makeCmd(CommandNames::name, func __VA_OPT__(,) __VA_ARGS__)

std::unique_ptr<CanvasCommand> addLayer() {
    return std::make_unique<CanvasCommand>(
        CommandNames::AddLayer, [](Canvas& canvas) {
            canvas.addLayer();
        }
    );
}

std::unique_ptr<CanvasCommand> deleteLayer(const size_t layerID) {
    return std::make_unique<CanvasCommand>(
        CommandNames::DeleteLayer, [layerID](Canvas& canvas) {
            canvas.deleteLayer(layerID);
        }
    );
}

std::unique_ptr<CanvasCommand> duplicateLayer(size_t layerID) {
    return std::make_unique<CanvasCommand>(
        CommandNames::DuplicateLayer, [layerID](Canvas& canvas) {
            canvas.duplicateLayer(layerID);
        }
    );
}

std::unique_ptr<CanvasCommand> mergeWithLayerBelow(size_t layerID) {
    return std::make_unique<CanvasCommand>(
        CommandNames::MergeLayer, [layerID](Canvas& canvas) {
            canvas.mergeWithLayerBelow(layerID);
        }
    );
}

std::unique_ptr<CanvasCommand> mergeAllLayers() {
    return std::make_unique<CanvasCommand>(
        CommandNames::Flatten, [](Canvas& canvas) {
            canvas.mergeAllLayers();
        }
    );
}

std::unique_ptr<CanvasCommand> moveLayerToIndex(size_t layerID, size_t index) {
    return std::make_unique<CanvasCommand>(
        CommandNames::Reorder, [layerID, index](Canvas& canvas) {
            canvas.moveLayerToIndex(layerID, index);
        }
    );
}

std::unique_ptr<CanvasCommand> rotateLeft() {
    return std::make_unique<CanvasCommand>(
        CommandNames::RotateLeft, [](Canvas& canvas) {
            canvas.rotateLeft();
        }
    );
}

std::unique_ptr<CanvasCommand> rotateRight() {
    return std::make_unique<CanvasCommand>(
        CommandNames::RotateRight, [](Canvas& canvas) {
            canvas.rotateRight();
        }
    );
}

std::unique_ptr<CanvasCommand> rotate180() {
    DEFINE_COMMAND(Rotate180, Effects::rotate180);
}

std::unique_ptr<CanvasCommand> flipHorizCanvas() {
    DEFINE_COMMAND(FlipHorizontally, Effects::flipHorizontallyCanvas);
}

std::unique_ptr<CanvasCommand> flipVertCanvas() {
    DEFINE_COMMAND(FlipVertically, Effects::flipHorizontallyCanvas);
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

std::unique_ptr<CanvasCommand> seamCarving(const uint32_t width, const uint32_t height) {
    DEFINE_COMMAND(SeamCarving, Filters::seamCarving, width, height);
}

#pragma GCC diagnostic pop

}
