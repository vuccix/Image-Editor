#include <View/Utils.h>
#include <Assets/AssetManager.h>

namespace Utils {

void openURL(const std::string& url) {
#ifdef _WIN32
    const std::string command = "start \"\" \"" + url + "\"";
#elif __APPLE__
    const std::string command = "open \"" + url + "\"";
#else
    const std::string command = "xdg-open \"" + url + "\"";
#endif

    std::system(command.c_str());
}

void loadAssets(AssetManager& assetManager) {
    // checkerboard ----------------------------------------------------------------------------------------------------
    {
        constexpr uint8_t checkerData[16] = {
            200, 200, 200, 255,   255, 255, 255, 255,
            255, 255, 255, 255,   200, 200, 200, 255
        };

        assetManager.load("transparent", 2, 2, checkerData);
    }

    // ...
}

}
