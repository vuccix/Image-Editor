#include <Processing/Effects.h>
#include <Canvas/Canvas.h>
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

void Effects::flipHorizontally(Canvas& canvas) {
    for (size_t i = 0; i < canvas.layerCount(); ++i)
        flipHorizontally(canvas[i]);
}

void Effects::flipVertically(Canvas& canvas) {
    for (size_t i = 0; i < canvas.layerCount(); ++i)
        flipVertically(canvas[i]);
}

void Effects::rotateLeft(Canvas& canvas) {
    auto rotate = [](Layer& layer) -> std::vector<Pixel> {
        std::vector result(layer.width() * layer.height(), Pixel{});

        const auto newPixels = std::mdspan(result.data(), layer.width(), layer.height());
        const auto pixels    = layer.pixels();

        #pragma omp parallel for collapse(2)
        for (uint32_t y = 0; y < layer.height(); ++y)
            for (uint32_t x = 0; x < layer.width(); ++x)
                newPixels[layer.width() - 1 - x, y] = pixels[y, x];

        return result;
    };

    for (size_t i = 0; i < canvas.layerCount(); ++i) {
        std::vector<Pixel> layer = rotate(canvas[i]);
        canvas[i].setData(std::move(layer), canvas[i].height(), canvas[i].width());
    }

    // goofy ahh solution
    canvas.flipDimensions(); // TODO: think of something better
}

void Effects::rotateRight(Canvas& canvas) {
    auto rotate = [](Layer& layer) -> std::vector<Pixel> {
        std::vector result(layer.width() * layer.height(), Pixel{});

        const auto newPixels = std::mdspan(result.data(), layer.width(), layer.height());
        const auto pixels    = layer.pixels();

        #pragma omp parallel for collapse(2)
        for (uint32_t y = 0; y < layer.height(); ++y)
            for (uint32_t x = 0; x < layer.width(); ++x)
                newPixels[x, layer.height() - 1 - y] = pixels[y, x];

        return result;
    };

    for (size_t i = 0; i < canvas.layerCount(); ++i) {
        std::vector<Pixel> layer = rotate(canvas[i]);
        canvas[i].setData(std::move(layer), canvas[i].height(), canvas[i].width());
    }

    // goofy ahh solution
    canvas.flipDimensions(); // TODO: think of something better
}

void Effects::rotate180(Canvas& canvas) {
    for (size_t i = 0; i < canvas.layerCount(); ++i) {
        flipHorizontally(canvas[i]);
        flipVertically(canvas[i]);
    }
}

void Effects::brightness(Layer& image, const int value) {
    for (Pixel& pixel : image.data()) {
        pixel.r = static_cast<uint8_t>(std::clamp(pixel.r + value, 0, 255));
        pixel.g = static_cast<uint8_t>(std::clamp(pixel.g + value, 0, 255));
        pixel.b = static_cast<uint8_t>(std::clamp(pixel.b + value, 0, 255));
    }
}

void Effects::contrast(Layer& image, const float factor) {
    for (Pixel& pixel : image.data()) {
        const float value[3] = {
            factor * static_cast<float>(pixel.r - 128) + 128.f,
            factor * static_cast<float>(pixel.g - 128) + 128.f,
            factor * static_cast<float>(pixel.b - 128) + 128.f,
        };

        pixel.r = static_cast<uint8_t>(std::clamp(value[0], 0.f, 255.f));
        pixel.g = static_cast<uint8_t>(std::clamp(value[1], 0.f, 255.f));
        pixel.b = static_cast<uint8_t>(std::clamp(value[2], 0.f, 255.f));
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

        pixel.r           = static_cast<uint8_t>(std::clamp(new_r, 0.f, 255.f));
        pixel.g           = static_cast<uint8_t>(std::clamp(new_g, 0.f, 255.f));
        pixel.b           = static_cast<uint8_t>(std::clamp(new_b, 0.f, 255.f));
    }
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

    for (Pixel& pixel : image.data()) {
        const float value = pixel.r * arr[0]
                          + pixel.g * arr[1]
                          + pixel.b * arr[2];

        pixel.r           = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f));
        pixel.g           = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f));
        pixel.b           = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f));
    }
}

void Effects::luminance(Layer& image) {
    constexpr std::array arr = { 0.2126f, 0.7152f, 0.0722f };

    for (Pixel& pixel : image.data()) {
        const float value = pixel.r * arr[0]
                          + pixel.g * arr[1]
                          + pixel.b * arr[2];

        pixel.r           = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f));
        pixel.g           = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f));
        pixel.b           = static_cast<uint8_t>(std::clamp(value, 0.f, 255.f));
    }
}

void Effects::sepia(Layer& image) {
    constexpr std::array m = {
        0.393f, 0.769f, 0.189f,
        0.349f, 0.686f, 0.168f,
        0.272f, 0.534f, 0.131f
    };

    for (Pixel& pixel : image.data()) {
        const float new_r = m[0] * pixel.r + m[1] * pixel.g + m[2] * pixel.b;
        const float new_g = m[3] * pixel.r + m[4] * pixel.g + m[5] * pixel.b;
        const float new_b = m[6] * pixel.r + m[7] * pixel.g + m[8] * pixel.b;

        pixel.r           = static_cast<uint8_t>(std::clamp(new_r, 0.f, 255.f));
        pixel.g           = static_cast<uint8_t>(std::clamp(new_g, 0.f, 255.f));
        pixel.b           = static_cast<uint8_t>(std::clamp(new_b, 0.f, 255.f));
    }
}
