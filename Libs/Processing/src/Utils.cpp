#include "Utils.h"
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <vector>
#include <omp.h>

template <typename T>
void Utils::convolution(Layer& image, std::mdspan<const T, std::dextents<size_t, 2>> kernel) {
    std::vector result(image.width() * image.height(), Pixel{});

    const auto rows    = static_cast<int32_t>(image.height());
    const auto cols    = static_cast<int32_t>(image.width());
    const auto kHeight = static_cast<int32_t>(kernel.extent(0));
    const auto kWidth  = static_cast<int32_t>(kernel.extent(1));

    const int32_t rW   = kWidth  / 2;
    const int32_t rH   = kHeight / 2;

    auto index = [](const int32_t id, const int32_t max) -> int32_t {
        return (id < 0) ? -id - 1 : (id >= max) ? 2 * max - id - 1 : id;
    };

    const auto pixels = image.pixels();
    auto sumKernel = [&](const int32_t x, const int32_t y) {
        std::array<T, 3> sum = { 0, 0, 0 };

        for (int32_t ky = -rH; ky <= rH; ++ky) {
            const int32_t iy = index(y + ky, rows);
            const int32_t ki = ky + rH;

            for (int32_t kx = -rW; kx <= rW; ++kx) {
                const int32_t ix = index(x + kx, cols);
                const int32_t kj = kx + rW;

                const T       v  = kernel[ki, kj];
                const Pixel&  p  = pixels[iy, ix];

                sum[0] += p.r * v;
                sum[1] += p.g * v;
                sum[2] += p.b * v;
            }
        }

        return sum;
    };

    const auto newPixels = std::mdspan(result.data(), rows, cols);

    #pragma omp parallel for collapse(2)
    for (int32_t y = 0; y < rows; ++y) {
        for (int32_t x = 0; x < cols; ++x) {
            const auto [sumR, sumG, sumB] = sumKernel(x, y);
            constexpr T lo = 0, hi = 255;

            newPixels[y, x] = {
                .r = static_cast<uint8_t>(std::clamp(sumR, lo, hi)),
                .g = static_cast<uint8_t>(std::clamp(sumG, lo, hi)),
                .b = static_cast<uint8_t>(std::clamp(sumB, lo, hi)),
                .a = pixels[y, x].a
            };
        }
    }

    image.setData(std::move(result));
}

template void Utils::convolution<int32_t>(Layer& image, std::mdspan<const int32_t, std::dextents<size_t, 2>> kernel);
template void Utils::convolution<float  >(Layer& image, std::mdspan<const float,   std::dextents<size_t, 2>> kernel);
template void Utils::convolution<double >(Layer& image, std::mdspan<const double,  std::dextents<size_t, 2>> kernel);
