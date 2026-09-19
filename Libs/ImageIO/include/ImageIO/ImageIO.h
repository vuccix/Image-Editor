#pragma once

#include <filesystem>
#include <expected>

struct Image;

namespace ImageIO {

    enum class Error {
        CannotOpenFile,
        UnsupportedFormat,
        InvalidDimensions,
        WriteFailed
    };

    enum class ImageFormat {
        PNG, JPEG, TGA, GIF, WebP,
    };

    struct SaveOptions {
        ImageFormat format      = {};
        int32_t     jpegQuality = 95;
    };

    std::expected<Image, Error> load(const std::filesystem::path& path);
    std::expected<void,  Error> save(const std::filesystem::path& path, const Image& image, SaveOptions options = {});

    void showError(Error err);

}
