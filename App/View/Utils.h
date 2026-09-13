#pragma once

#include <string>

class  AssetManager;
struct EditorState;
class  Controller;

namespace Utils {

    void openURL(const std::string& url);

    void loadAssets(AssetManager& assetManager);

    // Layer Operations ================================================================================================

    void addLayer(EditorState& state, Controller& controller);
    void duplicateLayer(EditorState& state, Controller& controller);

}
