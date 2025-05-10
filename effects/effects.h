#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <unordered_set>

namespace Effects {
    using RGB_IMAGE = std::vector<std::vector<std::array<uint8_t, 3>>>;
    using GRAYSCALE = std::vector<std::vector<float>>;
    using LUMINANCE = GRAYSCALE;
    using SEPIA     = RGB_IMAGE;

    static const std::unordered_set<const char*> AVAILABLE_EFFECTS = {
        "grayscale",
        "sepia",
        "luminance",
        "invert",
        "seam_carving",
    };

    #define HAS(x) AVAILABLE_EFFECTS.contains(x)

    void invert(RGB_IMAGE& img);
    void sepia(RGB_IMAGE& img);
    GRAYSCALE grayscale(const RGB_IMAGE& img);
    LUMINANCE luminance(const RGB_IMAGE& img);
    RGB_IMAGE seam_carving(RGB_IMAGE& img, int iterCnt, bool isAccurate);
}
