#include <Serialization/Serialize.h>
#include <Canvas/Canvas.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <nfd.hpp>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <array>

namespace fs = std::filesystem;

namespace {

enum class Path { Load, Save };

std::optional<fs::path> getPath(const ::Path pathType) {
    NFD::Guard      nfdGuard;
    NFD::UniquePath outPath;

    // filters for dialog
    constexpr std::array filterItem = {
        nfdfilteritem_t{ "PNG", "png" },
        nfdfilteritem_t{ "JPG", "jpg,jpeg,jfif" },
        nfdfilteritem_t{ "TGA", "tga" },
        nfdfilteritem_t{ "GIF", "gif" },
        // TODO: webp
    };

    // show dialog
    nfdresult_t result;
    if (pathType == Path::Load) result = NFD::OpenDialog(outPath, filterItem.data(), filterItem.size());
    else                        result = NFD::SaveDialog(outPath, filterItem.data(), filterItem.size());

    if (result == NFD_OKAY)
        return outPath.get();

    return std::nullopt;
}

}

void Serialize::loadImage(Canvas& canvas) {
    const auto filePath = ::getPath(::Path::Load);
    if (!filePath.has_value()) return;

    if (canvas.layerCount() > 1) {
        while (canvas.layerCount() > 1)
            canvas.deleteLayer(canvas.layerCount() - 1);
    }

    const std::string str = filePath.value().generic_string();

    int32_t w, h, ch;
    uint8_t* data = stbi_load(str.c_str(), &w, &h, &ch, 4);

    if (!data) {
        std::cerr << "Failed to load image\n";
        return;
    }

    canvas.resize(w, h);

    const uint8_t*  dataPtr = data;
    const std::mdspan layer = canvas[0].pixels();
    for (uint32_t y = 0; y < static_cast<uint32_t>(h); ++y) {
        for (uint32_t x = 0; x < static_cast<uint32_t>(w); ++x) {
            layer[y, x].r = *dataPtr++;
            layer[y, x].g = *dataPtr++;
            layer[y, x].b = *dataPtr++;
            layer[y, x].a = *dataPtr++;
        }
    }

    stbi_image_free(data);
}

void Serialize::saveImage(Canvas& canvas) {
    const auto path = ::getPath(::Path::Save);
    if (!path.has_value()) return;

    canvas.updateComposite();
    const auto& [pixels, width, height] = canvas.getComposite();
    const auto w = static_cast<int32_t>(width);
    const auto h = static_cast<int32_t>(height);

    const std::string str = path.value().generic_string();
    const std::string ext = path.value().extension().generic_string();

    if (ext == ".png") {
        if (stbi_write_png(str.c_str(), w, h, 4, pixels.data(), w * 4) == 0)
            std::cerr << "Failed to save image\n";
    }
    else if (ext == ".jpg") {
        if (stbi_write_jpg(str.c_str(), w, h, 4, pixels.data(), 100) == 0)
            std::cerr << "Failed to save image\n";
    }
    else if (ext == ".tga") {
        if (stbi_write_tga(str.c_str(), w, h, 4, pixels.data()) == 0)
            std::cerr << "Failed to save image\n";
    }
}
