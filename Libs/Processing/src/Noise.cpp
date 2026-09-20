#include <Processing/Filters.h>
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <random>
#include <omp.h>

namespace {

std::random_device g_seed;
std::mt19937 g_generator(::g_seed());

}

#define CLAMP(X)   static_cast<uint8_t>(std::clamp((X), 0,   255))
#define CLAMP_F(X) static_cast<uint8_t>(std::clamp((X), 0.f, 255.f))

void Filters::uniformNoise(Layer& image, const int32_t amount) {
    assert(amount > 0);

    std::uniform_int_distribution distribution(-amount, amount);
    const std::mdspan pixels = image.pixels();

    #pragma omp parallel for schedule(static)
    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            const auto [r, g, b, a] = pixels[y, x];

            pixels[y, x] = {
                .r = CLAMP(r + distribution(::g_generator)),
                .g = CLAMP(g + distribution(::g_generator)),
                .b = CLAMP(b + distribution(::g_generator)),
                .a = a
            };
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
            const auto [r, g, b, a] = pixels[y, x];

            pixels[y, x] = {
                .r = CLAMP_F(r + distribution(::g_generator)),
                .g = CLAMP_F(g + distribution(::g_generator)),
                .b = CLAMP_F(b + distribution(::g_generator)),
                .a = a
            };
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
            const auto [r, g, b, a] = pixels[y, x];
            const float bright      = 0.2126f * r + 0.7152f * g + 0.0722f * b;
            const float normBright  = bright * norm;

            const float grainAmount = strength * std::sqrt(std::max(normBright, 0.01f));
            const float grain       = noise(::g_generator) * grainAmount;

            pixels[y, x] = {
                .r = CLAMP_F(r + grain),
                .g = CLAMP_F(g + grain),
                .b = CLAMP_F(b + grain),
                .a = a
            };
        }
    }
}

#undef CLAMP
#undef CLAMP_F
