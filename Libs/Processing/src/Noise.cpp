#include <Processing/Filters.h>
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <random>
#include <omp.h>

namespace {

std::random_device g_seed;
std::mt19937 g_generator(::g_seed());

template <typename U>
constexpr uint8_t toByte(U value) {
    return static_cast<uint8_t>(std::clamp(value, U{0}, U{255}));
}

}

void Filters::uniformNoise(Layer& image, const int32_t amount) {
    assert(amount > 0);

    std::uniform_int_distribution distribution(-amount, amount);
    const std::mdspan pixels = image.pixels();

    #pragma omp parallel for schedule(static)
    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            Pixel& pixel = pixels[y, x];
            pixel.r      = ::toByte(pixel.r + distribution(::g_generator));
            pixel.g      = ::toByte(pixel.g + distribution(::g_generator));
            pixel.b      = ::toByte(pixel.b + distribution(::g_generator));
        }
    }
}

void Filters::gaussianNoise(Layer& image, const float amount) {
    assert(amount > 0);

    std::normal_distribution distribution(0.f, amount);
    const std::mdspan pixels = image.pixels();

    #pragma omp parallel for schedule(static)
    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            Pixel& pixel = pixels[y, x];
            pixel.r      = ::toByte(pixel.r + distribution(::g_generator));
            pixel.g      = ::toByte(pixel.g + distribution(::g_generator));
            pixel.b      = ::toByte(pixel.b + distribution(::g_generator));
        }
    }
}

void Filters::filmGrain(Layer& image, const float strength) {
    assert(strength > 0);

    std::normal_distribution noise(0.f, 1.f);
    const std::mdspan pixels = image.pixels();
    constexpr float   norm   = 1.f / 255.f;

    #pragma omp parallel for collapse(2) schedule(static)
    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            Pixel& pixel            = pixels[y, x];
            const float bright      = 0.2126f * pixel.r + 0.7152f * pixel.g + 0.0722f * pixel.b;
            const float normBright  = bright * norm;

            const float grainAmount = strength * std::sqrt(std::max(normBright, 0.01f));
            const float grain       = noise(::g_generator) * grainAmount;

            pixel.r                 = ::toByte(pixel.r + grain);
            pixel.g                 = ::toByte(pixel.g + grain);
            pixel.b                 = ::toByte(pixel.b + grain);
        }
    }
}
