#include "CanvasCommand.h"
#include <Model/EditorState.h>
#include <Processing/Filters.h>
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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Cmd {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"

std::unique_ptr<CanvasCommand> deleteLayer(const size_t layerID) {
    return std::make_unique<CanvasCommand>(
        CommandNames::DeleteLayer, [layerID](Canvas& canvas) {
            canvas.deleteLayer(layerID);
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
    return std::make_unique<CanvasCommand>(
        CommandNames::SeamCarving, [width, height](Canvas& canvas) {
            Filters::seamCarving(canvas, width, height);
        }
    );
}

#pragma GCC diagnostic pop

}
