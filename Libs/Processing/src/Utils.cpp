#include "Utils.h"
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <vector>

void Utils::convolution(Layer& image, const std::mdspan<const int32_t, std::dextents<size_t, 2>> kernel) {
    std::vector result(image.width() * image.height(), Pixel{});

    const auto rows    = static_cast<int32_t>(image.height());
    const auto cols    = static_cast<int32_t>(image.width());
    const auto kWidth  = static_cast<int32_t>(kernel.extent(0));
    const auto kHeight = static_cast<int32_t>(kernel.extent(1));

    const int32_t rW   = kWidth  / 2;
    const int32_t rH   = kHeight / 2;

    auto index = [](const int32_t id, const int32_t max) -> int32_t {
        return (id < 0) ? -id - 1 : (id >= max) ? 2 * max - id - 1 : id;
    };

    const auto pixels = image.pixels();
    auto sumKernel = [&](const int32_t x, const int32_t y) {
        std::array sum = { 0, 0, 0 };

        for (int32_t ky = -rH; ky <= rH; ++ky) {
            const int32_t iy = index(y + ky, rows);
            const int32_t ki = ky + rH;

            for (int32_t kx = -rW; kx <= rW; ++kx) {
                const int32_t ix = index(x + kx, cols);
                const int32_t kj = kx + rW;

                const int32_t v  = kernel[ki, kj];
                const Pixel&  p  = pixels[iy, ix];

                sum[0] += p.r * v;
                sum[1] += p.g * v;
                sum[2] += p.b * v;
            }
        }

        return sum;
    };

    const auto newPixels = std::mdspan(result.data(), image.height(), image.width());
    for (int32_t y = 0; y < rows; ++y) {
        for (int32_t x = 0; x < cols; ++x) {
            const auto [sumR, sumG, sumB] = sumKernel(x, y);
            newPixels[y, x] = {
                .r = static_cast<uint8_t>(std::clamp(sumR, 0, 255)),
                .g = static_cast<uint8_t>(std::clamp(sumG, 0, 255)),
                .b = static_cast<uint8_t>(std::clamp(sumB, 0, 255)),
                .a = newPixels[y, x].a
            };
        }
    }

    image.setData(std::move(result));
}
