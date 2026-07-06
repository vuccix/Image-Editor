#pragma once

#include <string>

class AssetManager;

namespace Utils {

    void openURL(const std::string& url);

    void loadAssets(AssetManager& assetManager);

}
