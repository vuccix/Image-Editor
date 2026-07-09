#include <Processing/Filters.h>
#include <Processing/Effects.h>
#include <Canvas/Canvas.h>
#include <Canvas/Layer.h>
#include "Utils.h"
#include <cassert>
#include <algorithm>
#include <array>

void Filters::blur(Layer& image, const int amount) {
    assert(amount > 0);

    const size_t kSize = 2 * amount + 1;
    const std::vector kernel(kSize, (1.f / kSize));

    const auto pixels = std::as_const(image).pixels();
    auto result       = Utils::convolution(pixels, std::mdspan(kernel.data(), 1, kernel.size()));

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

    const auto pixels = std::as_const(image).pixels();
    auto result       = Utils::convolution(pixels, std::mdspan(kernel, 3, 3));

    image.setData(std::move(result));
}

void Filters::outline(Layer& image) {
    constexpr int32_t kernel[] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1,
    };

    Effects::grayscale(image);

    const auto pixels = std::as_const(image).pixels();
    auto result       = Utils::convolution(pixels, std::mdspan(kernel, 3, 3));

    image.setData(std::move(result));
}
