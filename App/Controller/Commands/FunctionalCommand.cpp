#include "FunctionalCommand.h"
#include <Model/EditorState.h>
#include <Processing/Filters.h>
#include <Processing/Effects.h>

Cmd::FunctionalCommand::FunctionalCommand(const CommandNames name, std::move_only_function<void(EditorState&)> effectFunc,
                                                                   std::move_only_function<void(EditorState&)> invertFunc)
    : Command(name), m_effectFunc(std::move(effectFunc)), m_invertFunc(std::move(invertFunc)) {}

void Cmd::FunctionalCommand::execute(EditorState& state) { m_effectFunc(state); }
void Cmd::FunctionalCommand::undo(EditorState& state)    { m_invertFunc(state); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Cmd {

std::unique_ptr<Command> flipLayerHoriz() {
    return std::make_unique<FunctionalCommand>(
        CommandNames::FlipHorizontally,
        [](EditorState& state) { Effects::flipHorizontally(state.canvas[state.selectedLayerID]); },
        [](EditorState& state) { Effects::flipHorizontally(state.canvas[state.selectedLayerID]); }
    );
}

std::unique_ptr<Command> flipLayerVert() {
    return std::make_unique<FunctionalCommand>(
        CommandNames::FlipVertically,
        [](EditorState& state) { Effects::flipVertically(state.canvas[state.selectedLayerID]); },
        [](EditorState& state) { Effects::flipVertically(state.canvas[state.selectedLayerID]); }
    );
}

std::unique_ptr<Command> flipCanvasHoriz() {
    return std::make_unique<FunctionalCommand>(
        CommandNames::FlipHorizontally,
        [](EditorState& state) { Effects::flipHorizontally(state.canvas); },
        [](EditorState& state) { Effects::flipHorizontally(state.canvas); }
    );
}

std::unique_ptr<Command> flipCanvasVert() {
    return std::make_unique<FunctionalCommand>(
        CommandNames::FlipVertically,
        [](EditorState& state) { Effects::flipVertically(state.canvas); },
        [](EditorState& state) { Effects::flipVertically(state.canvas); }
    );
}

std::unique_ptr<Command> rotateLeft() {
    return std::make_unique<FunctionalCommand>(
        CommandNames::RotateLeft,
        [](EditorState& state) { state.canvas.rotateLeft();  },
        [](EditorState& state) { state.canvas.rotateRight(); }
    );
}

std::unique_ptr<Command> rotateRight() {
    return std::make_unique<FunctionalCommand>(
        CommandNames::RotateRight,
        [](EditorState& state) { state.canvas.rotateRight(); },
        [](EditorState& state) { state.canvas.rotateLeft();  }
    );
}

std::unique_ptr<Command> rotate180() {
    return std::make_unique<FunctionalCommand>(
        CommandNames::Rotate180,
        [](EditorState& state) { Effects::rotate180(state.canvas); },
        [](EditorState& state) { Effects::rotate180(state.canvas); }
    );
}

std::unique_ptr<Command> invert() {
    return std::make_unique<FunctionalCommand>(
        CommandNames::InvertColors,
        [](EditorState& state) { Effects::invert(state.canvas[state.selectedLayerID]); },
        [](EditorState& state) { Effects::invert(state.canvas[state.selectedLayerID]); }
    );
}

std::unique_ptr<Command> invertAlpha() {
    return std::make_unique<FunctionalCommand>(
        CommandNames::InvertAlpha,
        [](EditorState& state) { Effects::invertAlpha(state.canvas[state.selectedLayerID]); },
        [](EditorState& state) { Effects::invertAlpha(state.canvas[state.selectedLayerID]); }
    );
}

std::unique_ptr<Command> swapChannels(const int32_t comb) {
    return std::make_unique<FunctionalCommand>(
        CommandNames::SwapChannels,
        [comb](EditorState& state) { Filters::swapChannels(state.canvas[state.selectedLayerID], comb); },
        [comb](EditorState& state) { Filters::swapChannels(state.canvas[state.selectedLayerID], comb); }
    );
}

std::unique_ptr<Command> addLayer() {
    return std::make_unique<FunctionalCommand>(
        CommandNames::AddLayer,
        [](EditorState& state) { state.canvas.addLayer(); },
        [](EditorState& state) { state.canvas.deleteLayer(state.canvas.layerCount() - 1); }
    );
}

std::unique_ptr<Command> duplicateLayer(const size_t layerID) {
    return std::make_unique<FunctionalCommand>(
        CommandNames::DuplicateLayer,
        [layerID](EditorState& state) { state.canvas.duplicateLayer(layerID);  },
        [layerID](EditorState& state) { state.canvas.deleteLayer(layerID + 1); }
    );
}

std::unique_ptr<Command> moveLayerToIndex(const size_t layerID, const size_t index) {
    return std::make_unique<FunctionalCommand>(
        CommandNames::Reorder,
        [layerID, index](EditorState& state) { state.canvas.moveLayerToIndex(layerID, index); },
        [layerID, index](EditorState& state) { state.canvas.moveLayerToIndex(index, layerID); }
    );
}

std::unique_ptr<Command> changeOpacity(const float oldOpacity, const float newOpacity) {
    return std::make_unique<FunctionalCommand>(
        CommandNames::ChangeOpacity,
        [newOpacity](EditorState& s) { s.canvas[s.selectedLayerID].opacity = newOpacity; },
        [oldOpacity](EditorState& s) { s.canvas[s.selectedLayerID].opacity = oldOpacity; }
    );
}

std::unique_ptr<Command> changeFill(const float oldFill, const float newFill) {
    return std::make_unique<FunctionalCommand>(
        CommandNames::ChangeOpacity,
        [newFill](EditorState& s) { s.canvas[s.selectedLayerID].fill = newFill; },
        [oldFill](EditorState& s) { s.canvas[s.selectedLayerID].fill = oldFill; }
    );
}

}
