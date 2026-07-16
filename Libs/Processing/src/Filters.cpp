#include <Processing/Filters.h>
#include "Utils.h"
#include <Processing/Effects.h>
#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <omp.h>

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

    const std::mdspan img = image.pixels();
    #pragma omp parallel for collapse (2)
    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            Pixel& p = img[y,x];
            std::swap(p[id[0]], p[id[1]]);
        }
    }
}

void Filters::emboss(Layer& image) {
    constexpr int32_t kernel[] = { -2, -1, 0, /**/ -1,  1, 1, /**/ 0,  1, 2 };

    const std::vector clone(image.data().begin(), image.data().end());
    const std::mdspan src(clone.data(), image.height(), image.width());
    const std::mdspan dst = image.pixels();

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

    const std::vector clone(image.data().begin(), image.data().end());
    const std::mdspan src(clone.data(), image.height(), image.width());
    const std::mdspan dst = image.pixels();

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

namespace {

std::array<float, 25> getSharpenKernel(float amount) {
    // amount                  = std::clamp((amount * 0.01f), 0.f, 1.f);
    const float k           = amount * 3.f;

    constexpr int32_t size  = 5;
    constexpr int32_t r     = size / 2;
    constexpr float   sigma = 1.f;

    std::array<float, size * size> kernelData{};
    const std::mdspan kernel{kernelData.data(), size, size};

    float gaussSum = 0.f;
    for (int32_t y = -r; y <= r; ++y) {
        for (int32_t x = -r; x <= r; ++x) {
            const float exponent = -1.f * static_cast<float>(x * x + y * y) / (2.f * sigma * sigma);
            const float gaussVal = (1.f / (2.f * std::numbers::pi_v<float> * sigma * sigma)) * std::exp(exponent);

            kernel[y + r, x + r] = gaussVal;
            gaussSum            += gaussVal;
        }
    }

    for (int32_t y = 0; y < size; ++y) {
        for (int32_t x = 0; x < size; ++x) {
            const float gNorm = kernel[y, x] / gaussSum;

            if (y == r && x == r) kernel[y, x] = (1.f + k) - (k * gNorm);
            else                  kernel[y, x] = -k * gNorm;
        }
    }

    return kernelData;
}

}

void Filters::sharpen(Layer& image, const float amount) {
    assert(amount >= 0.f && amount <= 100.f);

    const std::vector clone(image.data().begin(), image.data().end());
    const std::mdspan src(clone.data(), image.height(), image.width());
    const std::mdspan dst    = image.pixels();

    const std::array  kernel = ::getSharpenKernel(amount);

    Utils::convolution(src, 5, 5, KernelOp{ std::mdspan(kernel.data(), 5, 5) },
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

void Filters::pixelate(Layer& image, const int blockSize) {
    assert(blockSize >= 1);

    const auto pixels = image.pixels();
    const auto rows   = static_cast<int32_t>(image.height());
    const auto cols   = static_cast<int32_t>(image.width());

    for (int32_t y = 0; y < rows; y += blockSize) {
        for (int32_t x = 0; x < cols; x += blockSize) {
            const int32_t maxY    = std::min(y + blockSize, rows);
            const int32_t maxX    = std::min(x + blockSize, cols);

            const int32_t centerY = y + (maxY - y) / 2;
            const int32_t centerX = x + (maxX - x) / 2;
            const Pixel   p       = pixels[centerY, centerX];

            for (int32_t iy = y; iy < maxY; ++iy) {
                for (int32_t jx = x; jx < maxX; ++jx) {
                    pixels[iy, jx] = {
                        .r = p.r,
                        .g = p.g,
                        .b = p.b,
                        .a = pixels[iy, jx].a
                    };
                }
            }
        }
    }
}

void Filters::duoTone(Layer& image, const float colorA[3], const float colorB[3]) {
    const std::mdspan pixels = image.pixels();

    #pragma omp parallel for collapse (2)
    for (uint32_t y = 0; y < image.height(); ++y) {
        for (uint32_t x = 0; x < image.width(); ++x) {
            Pixel& pixel     = pixels[y, x];
            const float lum  = (pixel.r * 0.2126f + pixel.g * 0.7152f + pixel.b * 0.0722f) / 255.f;
            const auto  r    = static_cast<uint8_t>(std::clamp(colorA[0] + (colorB[0] - colorA[0]) * lum, 0.f, 255.f));
            const auto  g    = static_cast<uint8_t>(std::clamp(colorA[1] + (colorB[1] - colorA[1]) * lum, 0.f, 255.f));
            const auto  b    = static_cast<uint8_t>(std::clamp(colorA[2] + (colorB[2] - colorA[2]) * lum, 0.f, 255.f));

            pixel = {
                .r = r,
                .g = g,
                .b = b,
                .a = pixel.a,
            };
        }
    }
}

void Filters::normalMap(Layer& image, const float strength, const bool flipY) {
    Effects::grayscale(image);

    const Utils::SobelData sobel = Utils::getSobel(image);

    auto normalize = [](float& x, float& y, float& z) {
        const float sum = x * x + y * y + z * z;
        if (sum <= 1e-8f) {
            x = y = 0.f;
            z = 1.f;
            return;
        }

        const float invLen = 1.f / std::sqrt(sum);
        x *= invLen;
        y *= invLen;
        z *= invLen;
    };

    const auto pixels = image.pixels();
    const auto rows   = static_cast<int32_t>(image.height());
    const auto cols   = static_cast<int32_t>(image.width());
    const float sign  = flipY ? -1.f : 1.f;

    #pragma omp parallel for collapse (2)
    for (int32_t y = 0; y < rows; ++y) {
        for (int32_t x = 0; x < cols; ++x) {
            const int32_t id = y * cols + x;
            const float   dx = sobel.Gx[id] * (1.f / 255.f) * strength;
            const float   dy = sobel.Gy[id] * (1.f / 255.f) * strength;

            // OpenGL-style normal
            float nx = -dx;
            float ny = -dy * sign;
            float nz = 1.f;

            normalize(nx, ny, nz);

            // [-1,1] -> [0,255]
            pixels[y, x] = {
                .r = static_cast<uint8_t>((nx * 0.5f + 0.5f) * 255.f),
                .g = static_cast<uint8_t>((ny * 0.5f + 0.5f) * 255.f),
                .b = static_cast<uint8_t>((nz * 0.5f + 0.5f) * 255.f),
                .a = pixels[y, x].a
            };
        }
    }
}
