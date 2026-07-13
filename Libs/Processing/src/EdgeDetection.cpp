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
    Effects::grayscale(image);
    Filters::gaussianBlur(image, 2);

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

namespace {

std::vector<float> nonMaximumSuppression(const std::vector<float>& mag,
                                         const std::vector<float>& gX,
                                         const std::vector<float>& gY,
                                         const int32_t width, const int32_t height) {
    std::vector suppressed(width * height, 0.f);
    const auto dst       = std::mdspan(suppressed.data(), height, width);
    const auto magnitude = std::mdspan(mag.data(), height, width);
    const auto gradY     = std::mdspan(gY.data(), height, width);
    const auto gradX     = std::mdspan(gX.data(), height, width);

    constexpr float PI    = std::numbers::pi_v<float>;
    constexpr float toDeg = 180.f / PI;

    for (int32_t y = 1; y < height - 1; ++y) {
        for (int32_t x = 1; x < width - 1; ++x) {
            float angle = std::atan2(gradY[y, x], gradX[y, x]) * toDeg;
            if (angle < 0.f) angle += 180.f;

            float q = 255.f;
            float r = 255.f;

            // round angle to nearest 0, 45, 90, or 135 degrees
            if ((0.f <= angle && angle < 22.5f) || (157.5f <= angle && angle <= 180.f)) {
                q = magnitude[y, x + 1];
                r = magnitude[y, x - 1];
            }
            else if (22.5f <= angle && angle < 67.5f) {
                q = magnitude[y + 1, x - 1];
                r = magnitude[y - 1, x + 1];
            }
            else if (67.5f <= angle && angle < 112.5f) {
                q = magnitude[y + 1, x];
                r = magnitude[y - 1, x];
            }
            else if (112.5f <= angle && angle < 157.5f) {
                q = magnitude[y - 1, x - 1];
                r = magnitude[y + 1, x + 1];
            }

            // keep pixel if it is local maximum along gradient direction
            dst[y, x] = (magnitude[y, x] >= q && magnitude[y, x] >= r) ? magnitude[y, x] : 0.f;
        }
    }
    return suppressed;
}

std::vector<float> hysteresis(const std::vector<float>& suppressed,
                              const int32_t width,        const int32_t height,
                              const float   lowThreshold, const float   highThreshold) {
    std::vector result(width * height, 0.f);
    const auto src = std::mdspan(suppressed.data(), height, width);
    const auto dst = std::mdspan(result.data(), height, width);

    constexpr float WEAK   = 100.f;
    constexpr float STRONG = 255.f;

    // double thresholding
    for (int32_t y = 0; y < height; ++y) {
        for (int32_t x = 0; x < width; ++x) {
            const float val = src[y, x];

            if      (val >= highThreshold) dst[y, x] = STRONG;
            else if (val >= lowThreshold)  dst[y, x] = WEAK;
            else                           dst[y, x] = 0.f;
        }
    }

    // hysteresis
    for (int32_t y = 1; y < height - 1; ++y) {
        for (int32_t x = 1; x < width - 1; ++x) {
            if (dst[y, x] == WEAK) {
                // check if any neighbor is a strong edge
                const bool hasStrongNeighbor = (dst[y - 1, x - 1] == STRONG) || (dst[y - 1, x + 0] == STRONG)
                                            || (dst[y - 1, x + 1] == STRONG) || (dst[y + 0, x - 1] == STRONG)
                                            || (dst[y + 0, x + 1] == STRONG) || (dst[y + 1, x - 1] == STRONG)
                                            || (dst[y + 1, x + 0] == STRONG) || (dst[y + 1, x + 1] == STRONG);

                dst[y, x] = hasStrongNeighbor ? STRONG : 0.f;
            }
        }
    }

    return result;
}

}

void Filters::canny(Layer& image, const float lowThreshold, const float highThreshold) {
    assert(0 <= lowThreshold && lowThreshold <= highThreshold && lowThreshold <= 100.f);

    Effects::grayscale(image);
    Filters::gaussianBlur(image, 2);

    const auto width  = static_cast<int32_t>(image.width());
    const auto height = static_cast<int32_t>(image.height());

    const auto [magnitude, Gx, Gy] = Utils::getSobel(image);
    const std::vector suppressed   = ::nonMaximumSuppression(magnitude, Gx, Gy, width, height);
    const std::vector result       = ::hysteresis(suppressed, width, height, lowThreshold, highThreshold);

    Utils::demote(std::span(result), image.data());
}
