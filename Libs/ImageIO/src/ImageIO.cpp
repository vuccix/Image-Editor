#include <ImageIO/ImageIO.h>
#include <Canvas/Image.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <iostream>
#include <string>

namespace {

namespace fs = std::filesystem;

std::string toLower(const std::string& s) {
    std::string result = s;

    for (size_t i = 1; i < s.length(); ++i)
        result[i] = static_cast<char>(std::tolower(s[i]));

    return result;
}

}

std::expected<Image, ImageIO::Error> ImageIO::load(const fs::path& path) {
    const std::string filePath = path.generic_string();
    const std::string ext      = ::toLower(path.extension().generic_string());
    constexpr int32_t channels = 4;

    if (ext != ".png" && ext != ".jpg" && ext != ".jpeg" && ext != ".jfif" && ext != ".tga" && ext != ".gif")
        return std::unexpected(Error::UnsupportedFormat);

    int32_t w, h, _;
    uint8_t* data = stbi_load(filePath.c_str(), &w, &h, &_, channels);

    if (!data)            return std::unexpected(Error::CannotOpenFile);
    if (w == 0 || h == 0) return std::unexpected(Error::InvalidDimensions);

    auto* pixels = reinterpret_cast<Pixel*>(data);

    Image result = {
        .pixels  = std::vector(pixels, pixels + (static_cast<size_t>(w) * h)),
        .width   = static_cast<uint32_t>(w),
        .height  = static_cast<uint32_t>(h),
    };

    stbi_image_free(data);

    return result;
}

std::expected<void, ImageIO::Error> ImageIO::save(const fs::path& path, const Image& image, const SaveOptions options) {
    const     int32_t w   = static_cast<int32_t>(image.width);
    const     int32_t h   = static_cast<int32_t>(image.height);
    constexpr int32_t ch  = 4;

    const std::string str = path.generic_string();

    switch (options.format) {
        case ImageFormat::PNG: {
            if (stbi_write_png(str.c_str(), w, h, ch, image.pixels.data(), w * ch) == 0)
                return std::unexpected(Error::WriteFailed);
            break;
        }
        case ImageFormat::JPEG: {
            if (stbi_write_jpg(str.c_str(), w, h, ch, image.pixels.data(), options.jpegQuality) == 0)
                return std::unexpected(Error::WriteFailed);
            break;
        }
        case ImageFormat::TGA: {
            if (stbi_write_tga(str.c_str(), w, h, ch, image.pixels.data()) == 0)
                return std::unexpected(Error::WriteFailed);
            break;
        }

        // GIF, WebP, ...

        default: throw std::runtime_error("Unknown format!");
    }

    return {};
}

void ImageIO::showError(const Error err) {
    switch (err) {
        case Error::CannotOpenFile:    std::cerr << "Cannot open file!\n";   break;
        case Error::UnsupportedFormat: std::cerr << "Unsupported format!\n"; break;
        case Error::InvalidDimensions: std::cerr << "Invalid dimensions!\n"; break;
        case Error::WriteFailed:       std::cerr << "Write failed!\n";       break;
    }
}
