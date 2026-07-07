#include <Processing/Effects.h>
#include <Canvas/Layer.h>

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
