#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <unordered_set>

namespace Effects {
    using PIXEL     = std::array<uint8_t, 3>;
    using RGB_IMAGE = std::vector<std::vector<PIXEL>>;

    using GRAYSCALE = std::vector<std::vector<float>>;
    using LUMINANCE = GRAYSCALE;

    void invert(RGB_IMAGE& img);
    void sepia(RGB_IMAGE& img);
    void seam_carving(RGB_IMAGE& img, int iterCnt, bool isAccurate);
    GRAYSCALE grayscale(const RGB_IMAGE& img);
    LUMINANCE luminance(const RGB_IMAGE& img);
    RGB_IMAGE rotate90deg(const RGB_IMAGE& img, bool side);
}
