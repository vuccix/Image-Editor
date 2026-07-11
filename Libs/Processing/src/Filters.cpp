#include <Processing/Filters.h>
#include <Processing/Effects.h>
#include <Canvas/Canvas.h>
#include <Canvas/Layer.h>
#include "Utils.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <array>

void Filters::blur(Layer& image, const int amount) {
    assert(amount > 0);

    const int32_t kSize = 2 * amount + 1;
    const std::vector kern(kSize, (1.f / kSize));

    struct Op {
        const Utils::std_mdspan<const float> kernel;

        std::array<float, 3> init() { return {}; }

        void accumulate(std::array<float, 3>& acc, const Pixel pixel, const int32_t ki, const int32_t kj) const {
            acc[0] += pixel.r * kernel[ki, kj];
            acc[1] += pixel.g * kernel[ki, kj];
            acc[2] += pixel.b * kernel[ki, kj];
        }
    };

    std::vector clone = image.copyData();
    std::mdspan src   = std::as_const(image).pixels();
    std::mdspan dst   = std::mdspan(clone.data(), image.height(), image.width());

    Utils::convolution(src, kSize, 1, Op{ std::mdspan(kern.data(), kern.size(), 1) },
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

    Utils::convolution(src, 1, kSize, Op{ std::mdspan(kern.data(), 1, kern.size()) },
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

    struct Op {
        const Utils::std_mdspan<const int32_t> kernel;

        std::array<int32_t, 3> init() { return {}; };

        void accumulate(std::array<int32_t, 3>& acc, const Pixel pixel, const int32_t ki, const int32_t kj) const {
            acc[0] += pixel.r * kernel[ki, kj];
            acc[1] += pixel.g * kernel[ki, kj];
            acc[2] += pixel.b * kernel[ki, kj];
        }
    };

    Utils::convolution(src, 3, 3, Op{ std::mdspan(kernel, 3, 3) },
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

    struct Op {
        const Utils::std_mdspan<const int32_t> kernel;

        std::array<int32_t, 3> init() { return {}; };

        void accumulate(std::array<int32_t, 3>& acc, const Pixel pixel, const int32_t ki, const int32_t kj) const {
            acc[0] += pixel.r * kernel[ki, kj];
            acc[1] += pixel.g * kernel[ki, kj];
            acc[2] += pixel.b * kernel[ki, kj];
        }
    };

    Utils::convolution(src, 3, 3, Op{ std::mdspan(kernel, 3, 3) },
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

void Filters::laplace(Layer& image) {
    constexpr int32_t kernel[] = { 1, 4, 1, /**/ 4, -20, 4, /**/ 1, 4, 1 };

    Filters::blur(image, 1);
    Effects::grayscale(image);

    const std::vector promoted = Utils::promote<int32_t>(image.data());
    const std::mdspan src      = std::mdspan(promoted.data(), image.height(), image.width());
    const std::mdspan dst      = image.pixels();

    struct LaplaceOp {
        const Utils::std_mdspan<const int32_t> kernel;

        int32_t init() { return 0; }

        void accumulate(int32_t& acc, const int32_t pixel, const int32_t ki, const int32_t kj) const {
            acc += pixel * kernel[ki, kj];
        }
    };

    Utils::convolution(src, 3, 3, LaplaceOp{ std::mdspan(kernel, 3, 3) },
        [&](const int32_t x, const int32_t y, const int32_t sum) {
            const uint8_t G = static_cast<uint8_t>(std::clamp(std::abs(sum), 0, 255));
            dst[y, x] = {
                .r = G,
                .g = G,
                .b = G,
                .a = dst[y, x].a
            };
        }
    );
}

namespace {

template <typename T>
using std_mdspan = std::mdspan<T, std::dextents<size_t, 2>>;

void edgeDetectionHelper(Layer& image, const std_mdspan<const int32_t> dx, const std_mdspan<const int32_t> dy) {
    Filters::blur(image, 1);
    Effects::grayscale(image);

    const std::vector promoted = Utils::promote<int32_t>(image.data());
    const std::mdspan src      = std::mdspan(promoted.data(), image.height(), image.width());
    const std::mdspan dst      = image.pixels();

    struct State {
        int32_t sumX, sumY;
    };

    struct GradientOp {
        const std_mdspan<const int32_t> dx;
        const std_mdspan<const int32_t> dy;

        State init() {
            return { 0, 0 };
        }

        void accumulate(State& state, const int32_t pixel, const int32_t ki, const int32_t kj) const {
            state.sumX += pixel * dx[ki, kj];
            state.sumY += pixel * dy[ki, kj];
        }
    };

    Utils::convolution(src, 3, 3, GradientOp{ dx, dy },
        [&](const int32_t x, const int32_t y, const State& state) {
            const int32_t G = std::abs(state.sumX) + std::abs(state.sumY);
            const uint8_t v = static_cast<uint8_t>(std::clamp(G, 0, 255));

            dst[y, x] = {
                .r = v,
                .g = v,
                .b = v,
                .a = dst[y, x].a
            };
        }
    );
}

}

void Filters::prewitt(Layer& image) {
    constexpr int32_t dx[] = { -1,  0,  1, /**/ -1, 0, 1, /**/ -1, 0, 1 };
    constexpr int32_t dy[] = { -1, -1, -1, /**/  0, 0, 0, /**/  1, 1, 1 };
    ::edgeDetectionHelper(image, std::mdspan(dx, 3, 3), std::mdspan(dy, 3, 3));
}

void Filters::scharr(Layer& image) {
    constexpr int32_t dx[] = { -3,   0,  3, /**/ -10, 0, 10, /**/ -3,  0, 3 };
    constexpr int32_t dy[] = { -3, -10, -3, /**/   0, 0,  0, /**/  3, 10, 3 };
    ::edgeDetectionHelper(image, std::mdspan(dx, 3, 3), std::mdspan(dy, 3, 3));
}

void Filters::sobel(Layer& image) {
    constexpr int32_t dx[] = { -1, 0, 1, /**/ -2, 0, 2, /**/ -1,  0,  1 };
    constexpr int32_t dy[] = {  1, 2, 1, /**/  0, 0, 0, /**/ -1, -2, -1 };
    ::edgeDetectionHelper(image, std::mdspan(dx, 3, 3), std::mdspan(dy, 3, 3));
}
