#include "Utils.h"
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>

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

// explicit instantiation ==============================================================================================

namespace Utils {

#define INSTANTIATE_PROMOTE(T)                                     \
    template std::vector<T> promote<T>(std::span<const Pixel>);    \

#define INSTANTIATE_DEMOTE(T)                                      \
    template void demote<T>(std::span<const T>, std::span<Pixel>); \

#define INSTANTIATE_CONVERSION(T)                                  \
    INSTANTIATE_PROMOTE(T)                                         \
    INSTANTIATE_DEMOTE(T)                                          \

// promote/demote
INSTANTIATE_CONVERSION(int32_t)
INSTANTIATE_CONVERSION(float)

#undef INSTANTIATE_PROMOTE
#undef INSTANTIATE_DEMOTE
#undef INSTANTIATE_CONVERSION

}

// =====================================================================================================================
