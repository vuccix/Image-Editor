#include "Utils.h"
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <omp.h>

template <typename T>
std::vector<T> Utils::promote(const std::span<const Pixel> pixels) {
    assert(pixels.empty() == false);

    std::vector<T> result(pixels.size(), T{});

    for (size_t i = 0; i < pixels.size(); ++i)
        result[i] = static_cast<T>(pixels[i].r);

    return result;
}

template <typename T>
void Utils::demote(const std::span<const T> data, const std::span<Pixel> pixels) {
    assert(data.size() == pixels.size());

    constexpr T lo = 0, hi = 255;

    for (size_t i = 0; i < data.size(); ++i) {
        pixels[i].r = static_cast<uint8_t>(std::clamp(data[i], lo, hi));
        pixels[i].g = static_cast<uint8_t>(std::clamp(data[i], lo, hi));
        pixels[i].b = static_cast<uint8_t>(std::clamp(data[i], lo, hi));
    }
}

template <typename T, typename U>
std::vector<T> Utils::convolution(const std_mdspan<const T> image, const std_mdspan<const U> kernel) {
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

    auto sumKernel = [=, &image, &kernel](const int32_t x, const int32_t y) {
        using Value = std::conditional_t<std::is_same_v<T, Pixel>, std::array<U, 3>, U>;

        Value sum{};

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
                    sum += p * v;
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
                const U sum     = sumKernel(x, y);
                newPixels[y, x] = sum;
            }
        }
    }

    return result;
}

// explicit instantiation ==============================================================================================

namespace Utils {

#define INSTANTIATE_CONVOLUTION(T, K)                                                    \
    template std::vector<T> convolution<T, K>(std_mdspan<const T>, std_mdspan<const K>); \

#define INSTANTIATE_PROMOTE(T)                                                           \
    template std::vector<T> promote<T>(std::span<const Pixel>);                          \

#define INSTANTIATE_DEMOTE(T)                                                            \
    template void demote<T>(std::span<const T>, std::span<Pixel>);                       \

#define INSTANTIATE_COMMON(T)                                                            \
    INSTANTIATE_CONVOLUTION(T, int32_t)                                                  \
    INSTANTIATE_CONVOLUTION(T, float)                                                    \

#define INSTANTIATE_CONVERSION(T)                                                        \
    INSTANTIATE_PROMOTE(T)                                                               \
    INSTANTIATE_DEMOTE(T)                                                                \

// convolution
INSTANTIATE_COMMON(Pixel)
INSTANTIATE_COMMON(int32_t)
INSTANTIATE_COMMON(float)

// promote/demote
INSTANTIATE_CONVERSION(int32_t)
INSTANTIATE_CONVERSION(float)

#undef INSTANTIATE_COMMON
#undef INSTANTIATE_CONVERSION

}

// =====================================================================================================================
