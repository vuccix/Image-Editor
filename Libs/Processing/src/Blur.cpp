#include <Processing/Filters.h>
#include "Utils.h"
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>

template <typename T>
using KernelOp = Utils::KernelOp<std::array<T, 3>, T>;

void Filters::blur(Layer& image, const int amount) {
    assert(amount > 0);

    const int32_t kSize = 2 * amount + 1;
    const std::vector kern(kSize, (1.f / kSize));

    std::vector clone = image.copyData();
    std::mdspan src   = std::as_const(image).pixels();
    std::mdspan dst   = std::mdspan(clone.data(), image.height(), image.width());

    Utils::convolution(src, kSize, 1, KernelOp{ std::mdspan(kern.data(), kern.size(), 1) },
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

    Utils::convolution(src, 1, kSize, KernelOp{ std::mdspan(kern.data(), 1, kern.size()) },
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
