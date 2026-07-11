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

    const size_t kSize = 2 * amount + 1;
    const std::vector kernel(kSize, (1.f / kSize));

    const auto  pixels = std::as_const(image).pixels();
    std::vector result = Utils::convolution(pixels, std::mdspan(kernel.data(), 1, kernel.size()));

    const auto pixels2 = std::mdspan(std::as_const(result).data(), image.height(), image.width());
    result             = Utils::convolution(pixels2, std::mdspan(kernel.data(), kernel.size(), 1));

    image.setData(std::move(result));
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
    constexpr int32_t kernel[] = {
        -2, -1, 0,
        -1,  1, 1,
         0,  1, 2,
    };

    const auto  pixels = std::as_const(image).pixels();
    std::vector result = Utils::convolution(pixels, std::mdspan(kernel, 3, 3));

    image.setData(std::move(result));
}

void Filters::outline(Layer& image) {
    constexpr int32_t kernel[] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1,
    };

    Effects::grayscale(image);

    const auto  pixels = std::as_const(image).pixels();
    std::vector result = Utils::convolution(pixels, std::mdspan(kernel, 3, 3));

    image.setData(std::move(result));
}

void Filters::laplace(Layer& image) {
    constexpr int32_t ker[] = { 1, 4, 1, /**/ 4, -20, 4, /**/ 1, 4, 1 };

    Filters::blur(image, 1);
    Effects::grayscale(image);

    const std::vector promoted = Utils::promote<int32_t>(image.data());
    const std::mdspan pixels   = std::mdspan(promoted.data(), image.height(), image.width());
    const std::span   data     = image.data();

    const std::vector L        = Utils::convolution(pixels, std::mdspan(ker, 3, 3));

    for (size_t i = 0; i < data.size(); ++i) {
        const uint8_t G = static_cast<uint8_t>(std::clamp(std::abs(L[i]), 0, 255));
        data[i] = {
            .r = G,
            .g = G,
            .b = G,
            .a = data[i].a
        };
    }
}

namespace {

void edgeDetectionHelper(Layer& image, const int32_t dx[], const int32_t dy[]) {
    Filters::blur(image, 1);
    Effects::grayscale(image);

    const std::vector promoted = Utils::promote<int32_t>(image.data());
    const std::mdspan pixels   = std::mdspan(promoted.data(), image.height(), image.width());
    const std::span   data     = image.data();

    const std::vector Gx       = Utils::convolution(pixels, std::mdspan(dx, 3, 3));
    const std::vector Gy       = Utils::convolution(pixels, std::mdspan(dy, 3, 3));

    for (size_t i = 0; i < data.size(); ++i) {
        const int32_t G = std::abs(Gx[i]) + std::abs(Gy[i]);
        const uint8_t v = static_cast<uint8_t>(std::clamp(G, 0, 255));
        data[i] = {
            .r = v,
            .g = v,
            .b = v,
            .a = data[i].a
        };
    }
}

}

void Filters::prewitt(Layer& image) {
    constexpr int32_t dx[] = { -1,  0,  1, /**/ -1, 0, 1, /**/ -1, 0, 1 };
    constexpr int32_t dy[] = { -1, -1, -1, /**/  0, 0, 0, /**/  1, 1, 1 };
    ::edgeDetectionHelper(image, dx, dy);
}

void Filters::scharr(Layer& image) {
    constexpr int32_t dx[] = { -3,   0,  3, /**/ -10, 0, 10, /**/ -3,  0, 3 };
    constexpr int32_t dy[] = { -3, -10, -3, /**/   0, 0,  0, /**/  3, 10, 3 };
    ::edgeDetectionHelper(image, dx, dy);
}

void Filters::sobel(Layer& image) {
    constexpr int32_t dx[] = { -1, 0, 1, /**/ -2, 0, 2, /**/ -1,  0,  1 };
    constexpr int32_t dy[] = {  1, 2, 1, /**/  0, 0, 0, /**/ -1, -2, -1 };
    ::edgeDetectionHelper(image, dx, dy);
}
