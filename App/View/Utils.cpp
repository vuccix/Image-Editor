#include <View/Utils.h>
#include <Assets/AssetManager.h>
#include <Model/EditorState.h>
#include <Controller/Controller.h>
#include <Controller/Commands/FunctionalCommand.h>

void Utils::openURL(const std::string& url) {
#ifdef _WIN32
    const std::string command = R"(start "" ")" + url + "\"";
#elif __APPLE__
    const std::string command = "open \"" + url + "\"";
#else
    const std::string command = "xdg-open \"" + url + "\"";
#endif

    std::system(command.c_str());
}

void Utils::loadAssets(AssetManager& assetManager) {
    // checkerboard ----------------------------------------------------------------------------------------------------
    {
        constexpr uint8_t checkerData[16] = {
            200, 200, 200, 255,   255, 255, 255, 255,
            255, 255, 255, 255,   200, 200, 200, 255
        };

        assetManager.load(2, 2, checkerData);
    }

    // ...
}

void Utils::addLayer(EditorState& state, Controller& controller) {
    controller.execute(state, Cmd::addLayer());
    state.selectedLayerID++;
}

void Utils::duplicateLayer(EditorState& state, Controller& controller) {
    controller.execute(state, Cmd::duplicateLayer(state.selectedLayerID));
    state.selectedLayerID++;
}
