#include <Processing/Filters.h>
#include "Utils.h"
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <complex>
#include <cmath>

template <typename T>
using KernelOp = Utils::KernelOp<std::array<T, 3>, T>;

namespace {

std::vector<float> getGaussianKernel(const int32_t sigma) {
    assert(sigma > 0);

    const uint32_t size    = 2 * sigma + 1;
    const int32_t  radius  = static_cast<int32_t>(size / 2);

    std::vector kernel(size, 0.f);

    const float twoSigmaSq = 2.f * sigma * sigma;
    const float constant   = 1.f / (std::numbers::pi_v<float> * twoSigmaSq);

    float sum = 0.f;

    for (int32_t x = -radius; x <= radius; ++x) {
        const float exponent = -1.f * static_cast<float>(x * x) / twoSigmaSq;
        const float value    = constant * std::exp(exponent);
        kernel[x + radius]   = value;
        sum                 += value;
    }

    const float scale = 1.f / sum;
    for (float& val : kernel)
        val *= scale;

    return kernel;
}

void blurHelper(Layer& image, const std::vector<float>& kernel, const int32_t kSize) {
    std::vector clone(image.data().begin(), image.data().end());
    std::mdspan src = std::as_const(image).pixels();
    std::mdspan dst(clone.data(), image.height(), image.width());

    Utils::convolution(src, kSize, 1, KernelOp{ std::mdspan(kernel.data(), kSize, 1) },
        [&](const int32_t x, const int32_t y, const std::array<float, 3> sum) {
            dst[y, x] = {
                .r = static_cast<uint8_t>(std::clamp(sum[0], 0.f, 255.f)),
                .g = static_cast<uint8_t>(std::clamp(sum[1], 0.f, 255.f)),
                .b = static_cast<uint8_t>(std::clamp(sum[2], 0.f, 255.f)),
                .a = dst[y, x].a
            };
        }
    );

    src = std::mdspan(clone.data(), image.height(), image.width());
    dst = image.pixels();

    Utils::convolution(src, 1, kSize, KernelOp{ std::mdspan(kernel.data(), 1, kSize) },
        [&](const int32_t x, const int32_t y, const std::array<float, 3> sum) {
            dst[y, x] = {
                .r = static_cast<uint8_t>(std::clamp(sum[0], 0.f, 255.f)),
                .g = static_cast<uint8_t>(std::clamp(sum[1], 0.f, 255.f)),
                .b = static_cast<uint8_t>(std::clamp(sum[2], 0.f, 255.f)),
                .a = dst[y, x].a
            };
        }
    );
}

}

void Filters::blur(Layer& image, const int32_t amount) {
    assert(amount > 0);

    const int32_t     kSize = 2 * amount + 1;
    const std::vector kernel(kSize, (1.f / kSize));

    ::blurHelper(image, kernel, kSize);
}

void Filters::gaussianBlur(Layer& image, const int32_t amount) {
    assert(amount > 0);

    const std::vector kernel = ::getGaussianKernel(amount);
    const int32_t     kSize  = 2 * amount + 1;

    ::blurHelper(image, kernel, kSize);
}

void Filters::motionBlur(Layer& image, int32_t distance, const float angle) {
    assert(distance > 0);

    if (distance % 2 == 0)
        distance += 1;

    // -----------------------------------------------------------------------------------------------------------------

    constexpr float   toRadian = std::numbers::pi_v<float> / 180.f;
    const     int32_t halfD    = distance / 2;
    const     float   stepX    = std::cos(angle * toRadian);
    const     float   stepY    = std::sin(angle * toRadian);

    std::vector<std::pair<int32_t, int32_t>> lineOffsets;
    lineOffsets.reserve(distance);

    for (int32_t t = -halfD; t <= halfD; ++t) {
        const auto offsetX = static_cast<int32_t>(std::round(t * stepX));
        const auto offsetY = static_cast<int32_t>(std::round(t * stepY));
        lineOffsets.emplace_back(offsetX, offsetY);
    }

    // -----------------------------------------------------------------------------------------------------------------

    const std::vector clone(image.data().begin(), image.data().end());
    const std::mdspan src(clone.data(), image.height(), image.width());
    const std::mdspan dst   = image.pixels();
    const float       scale = 1.f / static_cast<float>(lineOffsets.size());

    // -----------------------------------------------------------------------------------------------------------------

    const auto rows  = static_cast<int32_t>(image.height());
    const auto cols  = static_cast<int32_t>(image.width());

    auto index = [](const int32_t id, const int32_t max) -> int32_t {
        return (id < 0) ? -id - 1 : (id >= max) ? 2 * max - id - 1 : id;
    };

    #pragma omp parallel for collapse(2)
    for (int32_t y = 0; y < rows; ++y) {
        for (int32_t x = 0; x < cols; ++x) {
            std::array sum = { 0.f, 0.f, 0.f };

            for (const auto& [ox, oy] : lineOffsets) {
                const int32_t ix  = index(x + ox, cols);
                const int32_t iy  = index(y + oy, rows);

                const Pixel pixel = src[iy, ix];
                sum[0] += pixel.r;
                sum[1] += pixel.g;
                sum[2] += pixel.b;
            }

            dst[y, x] = {
                .r = static_cast<uint8_t>(std::clamp(sum[0] * scale, 0.f, 255.f)),
                .g = static_cast<uint8_t>(std::clamp(sum[1] * scale, 0.f, 255.f)),
                .b = static_cast<uint8_t>(std::clamp(sum[2] * scale, 0.f, 255.f)),
                .a = dst[y, x].a
            };
        }
    }
}
