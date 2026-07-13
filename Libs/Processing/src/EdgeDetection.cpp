#include <Processing/Filters.h>
#include "Utils.h"
#include <Processing/Effects.h>
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <cmath>

void Filters::laplace(Layer& image) {
    constexpr int32_t kernel[] = { 1, 4, 1, /**/ 4, -20, 4, /**/ 1, 4, 1 };

    Filters::gaussianBlur(image, 1);
    Effects::grayscale(image);

    const std::vector promoted = Utils::promote<int32_t>(image.data());
    const std::mdspan src      = std::mdspan(promoted.data(), image.height(), image.width());
    const std::mdspan dst      = image.pixels();

    Utils::convolution(src, 3, 3, Utils::KernelOp<int32_t, int32_t>{ std::mdspan(kernel, 3, 3) },
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
    Filters::gaussianBlur(image, 1);
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

        State init() { return { 0, 0 }; }

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
