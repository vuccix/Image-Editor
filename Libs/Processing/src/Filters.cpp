#include <Processing/Filters.h>
#include "Utils.h"
#include <Processing/Effects.h>
#include <Canvas/Canvas.h>
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <array>

template <typename T>
using KernelOp = Utils::KernelOp<std::array<T, 3>, T>;

void Filters::swapChannels(Layer& image, const int change) {
    assert(change >= 0 && change < 6);

    std::array id = { 0, 0 };
    switch (change) {
        case 0: id = { 0, 1 }; break; // 0: swap(R,G)
        case 1: id = { 0, 2 }; break; // 1: swap(R,B)
        case 2: id = { 0, 3 }; break; // 2: swap(R,A)
        case 3: id = { 1, 2 }; break; // 3: swap(G,B)
        case 4: id = { 1, 3 }; break; // 4: swap(G,A)
        case 5: id = { 2, 3 }; break; // 5: swap(B,A)
        default: break;
    }

    for (Pixel& pixel : image.data())
        std::swap(pixel[id[0]], pixel[id[1]]);
}

void Filters::emboss(Layer& image) {
    constexpr int32_t kernel[] = { -2, -1, 0, /**/ -1,  1, 1, /**/ 0,  1, 2 };

    const std::vector clone    = image.copyData();
    const std::mdspan src      = std::mdspan(clone.data(), image.height(), image.width());
    const std::mdspan dst      = image.pixels();

    Utils::convolution(src, 3, 3, KernelOp{ std::mdspan(kernel, 3, 3) },
        [&](const int32_t x, const int32_t y, const std::array<int32_t, 3> sum) {
            dst[y, x] = {
                .r = static_cast<uint8_t>(std::clamp(sum[0], 0, 255)),
                .g = static_cast<uint8_t>(std::clamp(sum[1], 0, 255)),
                .b = static_cast<uint8_t>(std::clamp(sum[2], 0, 255)),
                .a = dst[y, x].a
            };
        }
    );
}

void Filters::outline(Layer& image) {
    constexpr int32_t kernel[] = { -1, -1, -1, /**/ -1,  8, -1, /**/ -1, -1, -1 };

    Effects::grayscale(image);

    const std::vector clone = image.copyData();
    const std::mdspan src   = std::mdspan(clone.data(), image.height(), image.width());
    const std::mdspan dst   = image.pixels();

    Utils::convolution(src, 3, 3, KernelOp{ std::mdspan(kernel, 3, 3) },
        [&](const int32_t x, const int32_t y, const std::array<int32_t, 3> sum) {
            dst[y, x] = {
                .r = static_cast<uint8_t>(std::clamp(sum[0], 0, 255)),
                .g = static_cast<uint8_t>(std::clamp(sum[1], 0, 255)),
                .b = static_cast<uint8_t>(std::clamp(sum[2], 0, 255)),
                .a = dst[y, x].a
            };
        }
    );
}
