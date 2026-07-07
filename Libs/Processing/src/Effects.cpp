#include <Processing/Effects.h>
#include <Canvas/Layer.h>
#include <algorithm>
#include <array>
#include <omp.h>

void Effects::flipHorizontally(Layer& image) {
    const auto pixels = image.pixels();

    #pragma omp parallel for
    for (uint32_t y = 0; y < image.height(); ++y) {
        Pixel* rowStart = &pixels[y, 0];
        std::ranges::reverse(rowStart, rowStart + image.width());
    }
}

void Effects::flipVertically(Layer& image) {
    const auto pixels     = image.pixels();
    const uint32_t height = image.height();

    #pragma omp parallel for collapse(2)
    for (uint32_t y = 0; y < height / 2; ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            const uint32_t targetY = height - 1 - y;
            std::swap(pixels[y, x], pixels[targetY, x]);
        }
    }
}

void Effects::brightness(Layer& image, const int32_t value) {
    const auto pixels = image.pixels();

    for (uint32_t y = 0; y < image.height(); ++y)
        for (uint32_t x = 0; x < image.width(); ++x) {
            const auto [r, g, b, a] = pixels[y, x];
            pixels[y, x] = Pixel{
                .r = static_cast<uint8_t>(std::clamp((r + value), 0, 255)),
                .g = static_cast<uint8_t>(std::clamp((g + value), 0, 255)),
                .b = static_cast<uint8_t>(std::clamp((b + value), 0, 255)),
                .a = a
            };
        }
}

void Effects::contrast(Layer& image, const float factor) {
    const auto pixels = image.pixels();

    for (uint32_t y = 0; y < image.height(); ++y)
        for (uint32_t x = 0; x < image.width(); ++x) {
            const auto [r, g, b, a] = pixels[y, x];

            const float value[3] = {
                factor * static_cast<float>(r - 128) + 128.f,
                factor * static_cast<float>(g - 128) + 128.f,
                factor * static_cast<float>(b - 128) + 128.f,
            };

            pixels[y, x] = Pixel{
                .r = static_cast<uint8_t>(std::clamp(value[0], 0.f, 255.f)),
                .g = static_cast<uint8_t>(std::clamp(value[1], 0.f, 255.f)),
                .b = static_cast<uint8_t>(std::clamp(value[2], 0.f, 255.f)),
                .a = a
            };
        }
}

void Effects::saturation(Layer& image, const float factor) {
    constexpr std::array arr = { 0.2126f, 0.7152f, 0.0722f };
    const auto pixels        = image.pixels();

    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            const auto [r, g, b, a] = pixels[y, x];

            const float lum   = arr[0] * r + arr[1] * g + arr[2] * b;
            const float new_r = lum + factor * (static_cast<float>(r) - lum);
            const float new_g = lum + factor * (static_cast<float>(g) - lum);
            const float new_b = lum + factor * (static_cast<float>(b) - lum);

            pixels[y, x] = Pixel{
                .r = static_cast<uint8_t>(std::clamp(new_r, 0.f, 255.f)),
                .g = static_cast<uint8_t>(std::clamp(new_g, 0.f, 255.f)),
                .b = static_cast<uint8_t>(std::clamp(new_b, 0.f, 255.f)),
                .a = a
            };
        }
    }
}

void Effects::invert(Layer& image) {
    const auto pixels = image.pixels();

    for (uint32_t y = 0; y < image.height(); ++y)
        for (uint32_t x = 0; x < image.width(); ++x) {
            const auto [r, g, b, a] = pixels[y, x];
            pixels[y, x] = Pixel{
                .r = static_cast<uint8_t>(255 - r),
                .g = static_cast<uint8_t>(255 - g),
                .b = static_cast<uint8_t>(255 - b),
                .a = a
            };
        }
}

void Effects::invertAlpha(Layer& image) {
    const auto pixels = image.pixels();

    for (uint32_t y = 0; y < image.height(); ++y)
        for (uint32_t x = 0; x < image.width(); ++x) {
            const auto [r, g, b, a] = pixels[y, x];
            pixels[y, x] = Pixel{ r, g, b, static_cast<uint8_t>(255 - a) };
        }
}

void Effects::grayscale(Layer& image) {
    constexpr std::array arr = { 0.2989f, 0.5870f, 0.1140f };
    const auto pixels        = image.pixels();

    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            const auto [r, g, b, a] = pixels[y, x];

            const float value = r * arr[0]+ g * arr[1] + b * arr[2];

            pixels[y, x] = Pixel{
                .r = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f)),
                .g = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f)),
                .b = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f)),
                .a = a
            };
        }
    }
}

void Effects::luminance(Layer& image) {
    constexpr std::array arr = { 0.2126f, 0.7152f, 0.0722f };
    const auto pixels        = image.pixels();

    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            const auto [r, g, b, a] = pixels[y, x];

            const float value = r * arr[0]+ g * arr[1] + b * arr[2];

            pixels[y, x] = Pixel{
                .r = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f)),
                .g = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f)),
                .b = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f)),
                .a = a
            };
        }
    }
}

void Effects::sepia(Layer& image) {
    constexpr std::array m = {
        0.393f, 0.769f, 0.189f,
        0.349f, 0.686f, 0.168f,
        0.272f, 0.534f, 0.131f
    };

    const auto pixels = image.pixels();

    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            const auto [r, g, b, a] = pixels[y, x];

            const float new_r = m[0] * r + m[1] * g + m[2] * b;
            const float new_g = m[3] * r + m[4] * g + m[5] * b;
            const float new_b = m[6] * r + m[7] * g + m[8] * b;

            pixels[y, x] = Pixel{
                .r = static_cast<uint8_t>(std::clamp(new_r, 0.f, 255.f)),
                .g = static_cast<uint8_t>(std::clamp(new_g, 0.f, 255.f)),
                .b = static_cast<uint8_t>(std::clamp(new_b, 0.f, 255.f)),
                .a = a
            };
        }
    }
}
