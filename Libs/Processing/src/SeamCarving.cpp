#include <Processing/Filters.h>
#include "Utils.h"
#include <Processing/Effects.h>
#include <Canvas/Canvas.h>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <cmath>

namespace {

constexpr float INF = 1e20f;

struct ImageSize {
    int32_t width  = 0;
    int32_t height = 0;
    int32_t stride = 0;
};

float computeLocalEnergy(const std::span<const Pixel> image, const int32_t x, const int32_t y, const ImageSize size) {
    auto getLuminance = [](const Pixel p) -> float {
        return 0.2989f * p.r + 0.5870f * p.g + 0.1140f * p.b;
    };

    auto getL = [&](int32_t sampleX, int32_t sampleY) -> float {
        sampleX = std::clamp(sampleX, 0, size.width - 1);
        sampleY = std::clamp(sampleY, 0, size.height - 1);
        return getLuminance(image[sampleY * size.stride + sampleX]);
    };

    const float gx = -1 * getL(x - 1, y - 1) + 1 * getL(x + 1, y - 1) +
                     -2 * getL(x - 1, y + 0) + 2 * getL(x + 1, y)     +
                     -1 * getL(x - 1, y + 1) + 1 * getL(x + 1, y + 1) ;

    const float gy = -1 * getL(x - 1, y - 1) - 2 * getL(x + 0, y - 1) +
                     -1 * getL(x + 1, y - 1) + 1 * getL(x - 1, y + 1) +
                     +2 * getL(x + 0, y + 1) + 1 * getL(x + 1, y + 1) ;

    return std::abs(gx) + std::abs(gy);
}

void getCumulative(std::span<float> cumulativeData, std::span<const float> energyData, const ImageSize size) {
    const int32_t cols   = size.width;
    const int32_t rows   = size.height;
    const int32_t stride = size.stride;

    std::copy_n(energyData.begin(), cols, cumulativeData.begin());

    const std::mdspan cumulative(cumulativeData.data(), rows, stride);
    const std::mdspan energy(energyData.data(), rows, stride);

    for (int32_t y = 1; y < rows; ++y) {
        // left boundary (x = 0)
        {
            const float c    =            cumulative[y - 1, 0];
            const float r    = cols > 1 ? cumulative[y - 1, 1] : ::INF;

            cumulative[y, 0] = energy[y, 0] + std::min(c, r);
        }

        // middle
        for (int32_t x = 1; x < cols - 1; ++x) {
            const float l    = cumulative[y - 1, x - 1];
            const float c    = cumulative[y - 1, x];
            const float r    = cumulative[y - 1, x + 1];

            cumulative[y, x] = energy[y, x] + std::min({ l, c, r });
        }

        // right boundary (x = cols - 1)
        if (cols > 1) {
            const int32_t x  = cols - 1;
            const float   l  = cumulative[y - 1, x - 1];
            const float   c  = cumulative[y - 1, x];

            cumulative[y, x] = energy[y, x] + std::min(l, c);
        }
    }
}

void findSeam(std::span<int32_t> seam, const std::span<const float> cumulativeData, const ImageSize size) {
    const int32_t cols   = size.width;
    const int32_t rows   = size.height;
    const int32_t stride = size.stride;

    const std::mdspan cumulative(cumulativeData.data(), rows, stride);

    // find min value in bottom row
    float minVal = ::INF;
    for (int32_t x = 0; x < cols; ++x) {
        if (cumulative[rows - 1, x] < minVal) {
            seam[rows - 1] = x;
            minVal         = cumulative[rows - 1, x];
        }
    }

    // backtrack to find seam
    for (int32_t y = rows -1; y > 0; --y) {
        const int32_t seam_y = seam[y];

        const float l = (seam_y > 0)        ? cumulative[y - 1, seam_y - 1] : ::INF;
        const float c =                       cumulative[y - 1, seam_y + 0] ;
        const float r = (seam_y < cols - 1) ? cumulative[y - 1, seam_y + 1] : ::INF;

        if      (seam_y > 0 && l < c && l < r)        seam[y - 1] = seam_y - 1; // left
        else if (seam_y < cols - 1 && r < c && r < l) seam[y - 1] = seam_y + 1; // right
        else                                          seam[y - 1] = seam_y + 0; // center
    }
}

void removeSeam(const std::span<const int32_t> seam, std::span<float> energy, std::span<Pixel> image, const ImageSize size) {
    const int32_t rows   = size.height;
    const int32_t cols   = size.width;
    const int32_t stride = size.stride;

    #pragma omp parallel for
    for (int32_t y = 0; y < rows; ++y) {
        const int32_t seam_x = seam[y];

        // IMAGE
        Pixel* row = &image[y * stride];
        std::memmove(row + seam_x,
                     row + (seam_x + 1),
                     (cols - seam_x - 1) * sizeof(Pixel));

        // ENERGY
        float* e_row = &energy[y * stride];
        std::memmove(e_row + seam_x,
                     e_row + seam_x + 1,
                     (cols - seam_x - 1) * sizeof(float));

        // local energy update
        ImageSize nextSize = size;
        --nextSize.width;

        // update left pixel
        if (seam_x > 0)
            e_row[seam_x - 1] = computeLocalEnergy(image, seam_x - 1, y, nextSize);

        // update right pixel
        if (seam_x < nextSize.width)
            e_row[seam_x] = computeLocalEnergy(image, seam_x, y, nextSize);
    }
}

}

void Filters::seamCarving(Canvas& canvas, const unsigned inputWidth, const unsigned inputHeight) {
    assert(inputWidth > 0 && canvas.width() > inputWidth && inputHeight > 0 && canvas.height() > inputHeight);

    canvas.mergeAllLayers();
    Layer& layer = canvas[0];

    std::vector<Pixel> image(layer.data().begin(), layer.data().end());
    Effects::grayscale(layer);
    Filters::gaussianBlur(layer, 1);

    std::vector energy = Utils::getSobel(layer).magnitude;
    std::vector cumulative(layer.width() * layer.height(), 0.f);
    std::vector seam(layer.height(), 0);

    const uint32_t wIter = layer.width()  - inputWidth;
    // const uint32_t hIter = layer.height() - inputHeight;

    ImageSize imgSize = {
        .width  = static_cast<int32_t>(layer.width()),
        .height = static_cast<int32_t>(layer.height()),
        .stride = static_cast<int32_t>(layer.width())
    };

    for (uint32_t i = 0; i < wIter; ++i) {
        ::getCumulative(cumulative, energy, imgSize);
        ::findSeam(seam, cumulative, imgSize);
        ::removeSeam(seam, energy, image, imgSize);

        --imgSize.width;
    }

    canvas.resize(imgSize.width, imgSize.height);
    const auto dst = layer.pixels();

    for (int32_t y = 0; y < imgSize.height; ++y) {
        for (int32_t x = 0; x < imgSize.width; ++x)
            dst[y, x] = image[y * imgSize.stride + x];
    }
}
