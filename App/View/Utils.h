#pragma once

#include <filesystem>
#include <string>

class  AssetManager;
struct EditorState;
class  Controller;
class  Canvas;

namespace Utils {

    void openURL(const std::string& url);

    void loadAssets(AssetManager& assetManager);

    // layer operations ================================================================================================

    void addLayer(EditorState& state, Controller& controller);
    void duplicateLayer(EditorState& state, Controller& controller);

    // path selection ==================================================================================================

    void openImage(EditorState& state);
    void saveImage(Canvas& canvas);

}
