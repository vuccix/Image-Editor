#include "../effects.h"
#include "../../utils/convolution.h"
#include <cmath>
#include <vector>
#include <limits>

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

GRAYSCALE cumulativeEnergy(const GRAYSCALE& energy) {
    const int rows = static_cast<int>(energy.size());
    const int cols = static_cast<int>(energy[0].size());
    GRAYSCALE cumulative(rows, std::vector<float>(cols, 0.f));

    // copy first row
    cumulative[0] = energy[0];

    // compute cumulative energy
    for (int i = 1; i < rows; ++i) {
        for (int j{}; j < cols; ++j) {
            float left   = (j > 0)        ? cumulative[i - 1][j - 1] : std::numeric_limits<float>::max();
            float center =                  cumulative[i - 1][j + 0] ;
            float right  = (j < cols - 1) ? cumulative[i - 1][j + 1] : std::numeric_limits<float>::max();

            cumulative[i][j] = energy[i][j] + std::min(std::min(left, center), right);
        }
    }

    return cumulative;
}

std::vector<int> findSeam(const GRAYSCALE& cumulative) {
    const int rows = static_cast<int>(cumulative.size());
    const int cols = static_cast<int>(cumulative[0].size());
    std::vector<int> seam(rows, -1);

    // start at bottom row and find column with smallest value
    seam[rows - 1] = 0;
    for (int i{}; i < cols; ++i) {
        if (cumulative[rows - 1][i] < cumulative[rows - 1][seam[rows - 1]])
            seam[rows - 1] = i;
    }

    // backtrack to find seam path
    for (int i = rows - 1; i > 0; --i) {
        int curCol = seam[i]; // column id

        float left   = (curCol > 0)        ? cumulative[i - 1][curCol - 1] : -1;
        float center =                       cumulative[i - 1][curCol + 0] ;
        float right  = (curCol < cols - 1) ? cumulative[i - 1][curCol + 1] : -1;

        // left
        if (curCol > 0 && left < center && left < right)
            seam[i - 1] = curCol - 1;
        // right
        else if (curCol < cols - 1 && right < center && right < left)
            seam[i - 1] = curCol + 1;
        // center
        else
            seam[i - 1] = curCol;
    }

    return seam;
}

void removeSeam(RGB_IMAGE& img, const std::vector<int>& seam, GRAYSCALE& data) {
    const int rows = static_cast<int>(img.size());
    for (int i{}; i < rows; ++i) {
        img[i].erase(img[i].begin()   + seam[i]);
        data[i].erase(data[i].begin() + seam[i]);
    }
}

void Effects::seam_carving(RGB_IMAGE &img, const int iterCnt, const bool isAccurate = false) {
    GRAYSCALE imgGray = grayscale(img);
    GRAYSCALE energy  = energyMap(imgGray);

    for (int i{}; i < iterCnt; ++i) {
        GRAYSCALE cumulative  = cumulativeEnergy(energy);
        std::vector<int> seam = findSeam(cumulative);

        // just remove seam -- faster but less accurate
        if (!isAccurate)
            removeSeam(img, seam, energy);
        // remove seam and recompute energy map -- slower but more accurate
        else {
            removeSeam(img, seam, imgGray);
            energy = energyMap(imgGray);
        }
    }
}

GRAYSCALE Effects::sobelOperator(const RGB_IMAGE& img) {
    return energyMap(grayscale(img));
}
