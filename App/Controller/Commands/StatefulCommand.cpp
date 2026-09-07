#include "StatefulCommand.h"
#include "Model/EditorState.h"
#include "Canvas/Canvas.h"
#include <optional>
#include <cassert>

namespace Cmd {

std::unique_ptr<Command> deleteLayer(const size_t layerID) {
    assert(layerID != 0);

    struct DeletedLayer {
        std::optional<Layer> backup;
        size_t               index;
    };

    return std::make_unique<StatefulCommand<DeletedLayer>>(
        CommandNames::DeleteLayer,

        DeletedLayer {
            .backup = std::nullopt,
            .index  = layerID
        },

        [](EditorState& state, DeletedLayer& data) {
            Canvas& canvas = state.canvas;

            if (!data.backup.has_value())
                data.backup = canvas[data.index];

            canvas.deleteLayer(data.index);
        },

        [](EditorState& state, const DeletedLayer& data) {
            Canvas& canvas = state.canvas;
            canvas.addLayer(data.index, *data.backup);
        }
    );
}

std::unique_ptr<Command> mergeWithLayerBelow(const size_t topLayerID) {
    assert(topLayerID != 0);

    struct MergedLayers {
        std::optional<Layer> upper;
        std::optional<Layer> lower;
        size_t               upperID;
        size_t               lowerID;
    };

    return std::make_unique<StatefulCommand<MergedLayers>>(
        CommandNames::MergeLayer,

        MergedLayers {
            .upper   = std::nullopt,
            .lower   = std::nullopt,
            .upperID = topLayerID,
            .lowerID = topLayerID - 1
        },

        [](EditorState& state, MergedLayers& data) {
            Canvas& canvas = state.canvas;

            if (!data.upper.has_value())
                data.upper = canvas[data.upperID];

            if (!data.lower.has_value())
                data.lower = canvas[data.lowerID];

            canvas.mergeWithLayerBelow(data.upperID);
        },

        [](EditorState& state, const MergedLayers& data) {
            Canvas& canvas              = state.canvas;
            canvas[data.lowerID]        = *data.lower;
            canvas.addLayer(data.upperID, *data.upper);
        }
    );
}

}
