#include <Processing/Effects.h>
#include <Canvas/Layer.h>
#include <algorithm>
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
