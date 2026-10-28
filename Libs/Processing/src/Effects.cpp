#include <Processing/Effects.h>
#include <Canvas/Canvas.h>
#include <Canvas/Layer.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <omp.h>

namespace {

template <typename U>
constexpr uint8_t toByte(U value) {
    return static_cast<uint8_t>(std::clamp(value, U{0}, U{255}));
}

}

void Effects::flipHorizontally(Layer& image) {
    const std::mdspan pixels = image.pixels();

    #pragma omp parallel for
    for (uint32_t y = 0; y < image.height(); ++y) {
        Pixel* rowStart = &pixels[y, 0];
        std::ranges::reverse(rowStart, rowStart + image.width());
    }
}

void Effects::flipVertically(Layer& image) {
    const std::mdspan pixels = image.pixels();
    const uint32_t    height = image.height();

    #pragma omp parallel for collapse(2)
    for (uint32_t y = 0; y < height / 2; ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            const uint32_t targetY = height - 1 - y;
            std::swap(pixels[y, x], pixels[targetY, x]);
        }
    }
}

void Effects::flipHorizontally(Canvas& canvas) {
    for (Layer& layer : canvas)
        flipHorizontally(layer);
}

void Effects::flipVertically(Canvas& canvas) {
    for (Layer& layer : canvas)
        flipVertically(layer);
}

void Effects::rotate180(Canvas& canvas) {
    for (Layer& layer : canvas) {
        flipHorizontally(layer);
        flipVertically(layer);
    }
}

void Effects::brightness(Layer& image, const int32_t value) {
    for (Pixel& pixel : image.data()) {
        pixel.r = ::toByte(pixel.r + value);
        pixel.g = ::toByte(pixel.g + value);
        pixel.b = ::toByte(pixel.b + value);
    }
}

void Effects::contrast(Layer& image, const float factor) {
    for (Pixel& pixel : image.data()) {
        const float value[3] = {
            factor * static_cast<float>(pixel.r - 128) + 128.f,
            factor * static_cast<float>(pixel.g - 128) + 128.f,
            factor * static_cast<float>(pixel.b - 128) + 128.f,
        };

        pixel.r = ::toByte(value[0]);
        pixel.g = ::toByte(value[1]);
        pixel.b = ::toByte(value[2]);
    }
}

void Effects::saturation(Layer& image, const float factor) {
    constexpr std::array arr = { 0.2126f, 0.7152f, 0.0722f };

    for (Pixel& pixel : image.data()) {
        const float lum   = arr[0] * pixel.r
                          + arr[1] * pixel.g
                          + arr[2] * pixel.b;

        const float new_r = lum + factor * (static_cast<float>(pixel.r) - lum);
        const float new_g = lum + factor * (static_cast<float>(pixel.g) - lum);
        const float new_b = lum + factor * (static_cast<float>(pixel.b) - lum);

        pixel.r           = ::toByte(new_r);
        pixel.g           = ::toByte(new_g);
        pixel.b           = ::toByte(new_b);
    }
}

void Effects::toHSV(Layer& image) {
    auto convert = [](const Pixel rgb) -> Pixel {
        const float r    = rgb.r / 255.f;
        const float g    = rgb.g / 255.f;
        const float b    = rgb.b / 255.f;

        const float maxC = std::max(std::max(r, g), b);
        const float minC = std::min(std::min(r, g), b);

        const float v    = maxC;

        if (minC == maxC)
            return Pixel{ 0, 0, ::toByte(v * 255.f), rgb.a };

        const float s  = (maxC - minC) / maxC;

        const float rC = (maxC - r) / (maxC - minC);
        const float gC = (maxC - g) / (maxC - minC);
        const float bC = (maxC - b) / (maxC - minC);

        float h;

        if      (r == maxC) h = 0.f + bC - gC;
        else if (g == maxC) h = 2.f + rC - bC;
        else                h = 4.f + gC - rC;

        h = std::fmod(h / 6.f + 1.f, 1.f);

        return Pixel{
            .r = ::toByte(h * 255.f),
            .g = ::toByte(s * 255.f),
            .b = ::toByte(v * 255.f),
            .a = rgb.a
        };
    };

    const std::mdspan pixels = image.pixels();

    #pragma omp parallel for schedule(static)
    for (uint32_t y = 0; y < image.height(); ++y)
        for (uint32_t x = 0; x < image.width(); ++x)
            pixels[y, x] = convert(pixels[y, x]);
}

void Effects::toRGB(Layer& image) {
    auto convert = [](const Pixel hsv) -> Pixel {
        const float h = hsv.r / 255.f;
        const float s = hsv.g / 255.f;
        const float v = hsv.b / 255.f;

        if (s == 0.f) {
            const uint8_t value = ::toByte(v * 255.f);
            return Pixel{ value, value, value, hsv.a };
        }

        const float hue   = h * 6.f;
        const auto sector = static_cast<int32_t>(std::floor(hue));
        const float frac  = hue - sector;

        const float p     = v * (1.f - s);
        const float q     = v * (1.f - s * frac);
        const float t     = v * (1.f - s * (1.f - frac));

        float r, g, b;

        switch (sector % 6) {
            case 0:  r = v;   g = t;   b = p; break;
            case 1:  r = q;   g = v;   b = p; break;
            case 2:  r = p;   g = v;   b = t; break;
            case 3:  r = p;   g = q;   b = v; break;
            case 4:  r = t;   g = p;   b = v; break;
            default: r = v;   g = p;   b = q; break; // sector == 5
        }

        return Pixel{
            .r = ::toByte(r * 255.f),
            .g = ::toByte(g * 255.f),
            .b = ::toByte(b * 255.f),
            .a = hsv.a
        };
    };

    const std::mdspan pixels = image.pixels();

    #pragma omp parallel for schedule(static)
    for (uint32_t y = 0; y < image.height(); ++y)
        for (uint32_t x = 0; x < image.width(); ++x)
            pixels[y, x] = convert(pixels[y, x]);
}

void Effects::invert(Layer& image) {
    for (Pixel& pixel : image.data()) {
        pixel.r = static_cast<uint8_t>(255 - pixel.r);
        pixel.g = static_cast<uint8_t>(255 - pixel.g);
        pixel.b = static_cast<uint8_t>(255 - pixel.b);
    }
}

void Effects::invertAlpha(Layer& image) {
    for (Pixel& pixel : image.data())
        pixel.a = static_cast<uint8_t>(255 - pixel.a);
}

void Effects::grayscale(Layer& image) {
    constexpr std::array arr = { 0.2989f, 0.5870f, 0.1140f };
    const auto pixels        = image.pixels();

    #pragma omp parallel for schedule(static)
    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            Pixel& pixel       = pixels[y, x];
            const float value  = pixel.r * arr[0] + pixel.g * arr[1] + pixel.b * arr[2];
            const auto channel = ::toByte(value);

            pixel.r            = channel;
            pixel.g            = channel;
            pixel.b            = channel;
        }
    }
}

void Effects::luminance(Layer& image) {
    constexpr std::array arr = { 0.2126f, 0.7152f, 0.0722f };
    const auto pixels        = image.pixels();

    #pragma omp parallel for schedule(static)
    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            Pixel& pixel       = pixels[y, x];
            const float value  = pixel.r * arr[0] + pixel.g * arr[1] + pixel.b * arr[2];
            const auto channel = ::toByte(value);

            pixel.r            = channel;
            pixel.g            = channel;
            pixel.b            = channel;
        }
    }
}

void Effects::sepia(Layer& image) {
    constexpr std::array m = {
        0.393f, 0.769f, 0.189f,
        0.349f, 0.686f, 0.168f,
        0.272f, 0.534f, 0.131f
    };

    const std::mdspan pixels = image.pixels();

    #pragma omp parallel for schedule(static)
    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            Pixel& pixel      = pixels[y, x];
            const float new_r = m[0] * pixel.r + m[1] * pixel.g + m[2] * pixel.b;
            const float new_g = m[3] * pixel.r + m[4] * pixel.g + m[5] * pixel.b;
            const float new_b = m[6] * pixel.r + m[7] * pixel.g + m[8] * pixel.b;

            pixel.r           = ::toByte(new_r);
            pixel.g           = ::toByte(new_g);
            pixel.b           = ::toByte(new_b);
        }
    }
}
