#include <Processing/Filters.h>
#include "Utils.h"
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <complex>

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
    std::vector clone = image.copyData();
    std::mdspan src   = std::as_const(image).pixels();
    std::mdspan dst   = std::mdspan(clone.data(), image.height(), image.width());

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

void Filters::blur(Layer& image, const int amount) {
    assert(amount > 0);

    const int32_t     kSize = 2 * amount + 1;
    const std::vector kernel(kSize, (1.f / kSize));

    ::blurHelper(image, kernel, kSize);
}

void Filters::gaussianBlur(Layer& image, const int amount) {
    assert(amount > 0);

    const std::vector kernel = ::getGaussianKernel(amount);
    const int32_t     kSize  = 2 * amount + 1;

    ::blurHelper(image, kernel, kSize);
}
