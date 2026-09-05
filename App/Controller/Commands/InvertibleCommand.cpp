#include "InvertibleCommand.h"
#include <Model/EditorState.h>
#include <Processing/Filters.h>
#include <Processing/Effects.h>

Cmd::InvertibleCommand::InvertibleCommand(const CommandNames name, std::move_only_function<void(EditorState&)> effectFunc,
                                                                   std::move_only_function<void(EditorState&)> invertFunc)
    : Command(name), m_effectFunc(std::move(effectFunc)), m_invertFunc(std::move(invertFunc)) {}

void Cmd::InvertibleCommand::execute(EditorState& state) { m_effectFunc(state); }
void Cmd::InvertibleCommand::undo(EditorState& state)    { m_invertFunc(state); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Cmd {

std::unique_ptr<InvertibleCommand> flipLayerHoriz() {
    return std::make_unique<InvertibleCommand>(
        CommandNames::FlipHorizontally,
        [](EditorState& state) { Effects::flipHorizontally(state.canvas[state.selectedLayerID]); },
        [](EditorState& state) { Effects::flipHorizontally(state.canvas[state.selectedLayerID]); }
    );
}

std::unique_ptr<InvertibleCommand> flipLayerVert() {
    return std::make_unique<InvertibleCommand>(
        CommandNames::FlipVertically,
        [](EditorState& state) { Effects::flipVertically(state.canvas[state.selectedLayerID]); },
        [](EditorState& state) { Effects::flipVertically(state.canvas[state.selectedLayerID]); }
    );
}

std::unique_ptr<InvertibleCommand> flipCanvasHoriz() {
    return std::make_unique<InvertibleCommand>(
        CommandNames::FlipHorizontally,
        [](EditorState& state) { Effects::flipHorizontally(state.canvas); },
        [](EditorState& state) { Effects::flipHorizontally(state.canvas); }
    );
}

std::unique_ptr<InvertibleCommand> flipCanvasVert() {
    return std::make_unique<InvertibleCommand>(
        CommandNames::FlipVertically,
        [](EditorState& state) { Effects::flipVertically(state.canvas); },
        [](EditorState& state) { Effects::flipVertically(state.canvas); }
    );
}

std::unique_ptr<InvertibleCommand> rotateLeft() {
    return std::make_unique<InvertibleCommand>(
        CommandNames::RotateLeft,
        [](EditorState& state) { state.canvas.rotateLeft();  },
        [](EditorState& state) { state.canvas.rotateRight(); }
    );
}

std::unique_ptr<InvertibleCommand> rotateRight() {
    return std::make_unique<InvertibleCommand>(
        CommandNames::RotateRight,
        [](EditorState& state) { state.canvas.rotateRight(); },
        [](EditorState& state) { state.canvas.rotateLeft();  }
    );
}

std::unique_ptr<InvertibleCommand> rotate180() {
    return std::make_unique<InvertibleCommand>(
        CommandNames::Rotate180,
        [](EditorState& state) { Effects::rotate180(state.canvas); },
        [](EditorState& state) { Effects::rotate180(state.canvas); }
    );
}

std::unique_ptr<InvertibleCommand> invert() {
    return std::make_unique<InvertibleCommand>(
        CommandNames::InvertColors,
        [](EditorState& state) { Effects::invert(state.canvas[state.selectedLayerID]); },
        [](EditorState& state) { Effects::invert(state.canvas[state.selectedLayerID]); }
    );
}

std::unique_ptr<InvertibleCommand> invertAlpha() {
    return std::make_unique<InvertibleCommand>(
        CommandNames::InvertAlpha,
        [](EditorState& state) { Effects::invertAlpha(state.canvas[state.selectedLayerID]); },
        [](EditorState& state) { Effects::invertAlpha(state.canvas[state.selectedLayerID]); }
    );
}

std::unique_ptr<InvertibleCommand> swapChannels(const int32_t comb) {
    return std::make_unique<InvertibleCommand>(
        CommandNames::SwapChannels,
        [comb](EditorState& state) { Filters::swapChannels(state.canvas[state.selectedLayerID], comb); },
        [comb](EditorState& state) { Filters::swapChannels(state.canvas[state.selectedLayerID], comb); }
    );
}

std::unique_ptr<InvertibleCommand> addLayer() {
    return std::make_unique<InvertibleCommand>(
        CommandNames::AddLayer,
        [](EditorState& state) { state.canvas.addLayer(); },
        [](EditorState& state) { state.canvas.deleteLayer(state.canvas.layerCount() - 1); }
    );
}

std::unique_ptr<InvertibleCommand> duplicateLayer(const size_t layerID) {
    return std::make_unique<InvertibleCommand>(
        CommandNames::DuplicateLayer,
        [layerID](EditorState& state) { state.canvas.duplicateLayer(layerID);  },
        [layerID](EditorState& state) { state.canvas.deleteLayer(layerID + 1); }
    );
}

std::unique_ptr<InvertibleCommand> moveLayerToIndex(const size_t layerID, const size_t index) {
    return std::make_unique<InvertibleCommand>(
        CommandNames::Reorder,
        [layerID, index](EditorState& state) { state.canvas.moveLayerToIndex(layerID, index); },
        [layerID, index](EditorState& state) { state.canvas.moveLayerToIndex(index, layerID); }
    );
}

std::unique_ptr<InvertibleCommand> changeOpacity(const float oldOpacity, const float newOpacity) {
    return std::make_unique<InvertibleCommand>(
        CommandNames::ChangeOpacity,
        [newOpacity](EditorState& s) { s.canvas[s.selectedLayerID].opacity = newOpacity; },
        [oldOpacity](EditorState& s) { s.canvas[s.selectedLayerID].opacity = oldOpacity; }
    );
}

std::unique_ptr<InvertibleCommand> changeFill(const float oldFill, const float newFill) {
    return std::make_unique<InvertibleCommand>(
        CommandNames::ChangeOpacity,
        [newFill](EditorState& s) { s.canvas[s.selectedLayerID].fill = newFill; },
        [oldFill](EditorState& s) { s.canvas[s.selectedLayerID].fill = oldFill; }
    );
}

}
