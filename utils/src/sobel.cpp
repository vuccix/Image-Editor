#include "../sobel.h"
#include "../convolution.h"
#include <cmath>

GRAYSCALE sobel(const GRAYSCALE& img, bool axis) {
    static constexpr float dx[3][3] = { { -1.f,  0.f,  1.f }, { -2.f, 0.f, 2.f }, { -1.f, 0.f, 1.f } };
    static constexpr float dy[3][3] = { { -1.f, -2.f, -1.f }, {  0.f, 0.f, 0.f }, {  1.f, 2.f, 1.f } };

    const auto& kernel = !axis ? dx : dy;
    return convolution3x3(img, kernel);
}

GRAYSCALE energyMap(const GRAYSCALE& img) {
    // compute Sobel operators
    const GRAYSCALE Gx = sobel(img, false);
    const GRAYSCALE Gy = sobel(img, true);

    // compute energy map
    GRAYSCALE energy(img.size(), std::vector<float>(img[0].size(), 0.f));
    for (size_t i{}; i < energy.size(); ++i)
        for (size_t j{}; j < energy[0].size(); ++j)
            energy[i][j] = std::sqrt(Gx[i][j] * Gx[i][j] + Gy[i][j] * Gy[i][j]);

    return energy;
}
