#include "Utils.h"
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <omp.h>

template <typename T, typename U>
std::vector<T> Utils::convolution(std::mdspan<const T, std::dextents<size_t, 2>> image,
                                  std::mdspan<const U, std::dextents<size_t, 2>> kernel) {
    const auto rows    = static_cast<int32_t>(image.extent(0));
    const auto cols    = static_cast<int32_t>(image.extent(1));
    const auto kHeight = static_cast<int32_t>(kernel.extent(0));
    const auto kWidth  = static_cast<int32_t>(kernel.extent(1));

    const int32_t rW   = kWidth  / 2;
    const int32_t rH   = kHeight / 2;

    std::vector<T> result(rows * cols, T{});

    auto index = [](const int32_t id, const int32_t max) -> int32_t {
        return (id < 0) ? -id - 1 : (id >= max) ? 2 * max - id - 1 : id;
    };

    auto sumKernel = [&](const int32_t x, const int32_t y) {
        std::array<U, 3> sum = { 0, 0, 0 };

        for (int32_t ky = -rH; ky <= rH; ++ky) {
            const int32_t iy = index(y + ky, rows);
            const int32_t ki = ky + rH;

            for (int32_t kx = -rW; kx <= rW; ++kx) {
                const int32_t ix = index(x + kx, cols);
                const int32_t kj = kx + rW;

                const U       v  = kernel[ki, kj];
                const T&      p  = image[iy, ix];

                if constexpr (std::is_same_v<T, Pixel>) {
                    sum[0] += p.r * v;
                    sum[1] += p.g * v;
                    sum[2] += p.b * v;
                }
                else {
                    sum[0] += p * v;
                }
            }
        }

        return sum;
    };

    const auto newPixels = std::mdspan(result.data(), rows, cols);

    #pragma omp parallel for collapse(2)
    for (int32_t y = 0; y < rows; ++y) {
        for (int32_t x = 0; x < cols; ++x) {
            if constexpr (std::is_same_v<T, Pixel>) {
                const auto [sumR, sumG, sumB] = sumKernel(x, y);
                constexpr U lo = 0, hi = 255;

                newPixels[y, x] = {
                    .r = static_cast<uint8_t>(std::clamp(sumR, lo, hi)),
                    .g = static_cast<uint8_t>(std::clamp(sumG, lo, hi)),
                    .b = static_cast<uint8_t>(std::clamp(sumB, lo, hi)),
                    .a = image[y, x].a
                };
            }
            else {
                const auto sum  = sumKernel(x, y);
                newPixels[y, x] = sum[0];
            }
        }
    }

    return result;
}

// explicit instantiation ----------------------------------------------------------------------------------------------

namespace Utils {

#define INSTANTIATE_CONVOLUTION(T, K)                                                          \
    template std::vector<T> convolution<T, K>(std::mdspan<const T, std::dextents<size_t, 2>>,  \
                                              std::mdspan<const K, std::dextents<size_t, 2>>); \

#define INSTANTIATE_FOR_TYPE(T)         \
    INSTANTIATE_CONVOLUTION(T, int32_t) \
    INSTANTIATE_CONVOLUTION(T, float)   \
    INSTANTIATE_CONVOLUTION(T, double)  \

INSTANTIATE_FOR_TYPE(Pixel)
INSTANTIATE_FOR_TYPE(float)
INSTANTIATE_FOR_TYPE(double)

#undef INSTANTIATE_FOR_TYPE
#undef INSTANTIATE_CONVOLUTION

}

// ---------------------------------------------------------------------------------------------------------------------
