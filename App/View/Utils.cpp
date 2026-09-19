#include <View/Utils.h>
#include <Assets/AssetManager.h>
#include <Model/EditorState.h>
#include <Controller/Controller.h>
#include <Controller/Commands/FunctionalCommand.h>
#include <ImageIO/ImageIO.h>
#include <nfd.hpp>
#include <array>

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Utils::addLayer(EditorState& state, Controller& controller) {
    controller.execute(state, Cmd::addLayer());
    state.selectedLayerID++;
}

void Utils::duplicateLayer(EditorState& state, Controller& controller) {
    controller.execute(state, Cmd::duplicateLayer(state.selectedLayerID));
    state.selectedLayerID++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {

namespace fs = std::filesystem;

enum class Path { Load, Save };

std::optional<fs::path> getPath(const ::Path pathType) {
    NFD::Guard      nfdGuard;
    NFD::UniquePath outPath;

    // filters for dialog
    constexpr std::array filterItem = {
        nfdfilteritem_t{ "PNG", "png"           },
        nfdfilteritem_t{ "JPG", "jpg,jpeg,jfif" },
        nfdfilteritem_t{ "TGA", "tga"           },
        nfdfilteritem_t{ "GIF", "gif"           },
        // TODO: webp
    };

    // show dialog
    const nfdresult_t result = (pathType == ::Path::Load)
                             ? NFD::OpenDialog(outPath, filterItem.data(), filterItem.size())
                             : NFD::SaveDialog(outPath, filterItem.data(), filterItem.size());

    if (result == NFD_OKAY)
        return outPath.get();

    return std::nullopt;
}

}

void Utils::openImage(EditorState& state) {
    const auto path = ::getPath(::Path::Load);
    if (!path) return; // user cancelled

    auto image = ImageIO::load(*path);
    if (!image) {
        ImageIO::showError(image.error());
        return;
    }

    state.canvas.replaceWithImage(std::move(image.value()));
    state.selectedLayerID = 0;
    state.version++;
}

void Utils::saveImage(Canvas& canvas) {
    const auto path = ::getPath(::Path::Save);
    if (!path) return; // user cancelled

    canvas.updateComposite();

    const auto result = ImageIO::save(*path, canvas.getComposite());
    if (!result) ImageIO::showError(result.error());
}
