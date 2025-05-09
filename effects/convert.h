#pragma once

#include <vector>
#include <array>

using RGB_IMAGE = std::vector<std::vector<std::array<int, 3>>>;
using GRAYSCALE = std::vector<std::vector<float>>;
using LUMINANCE = GRAYSCALE;
using SEPIA     = RGB_IMAGE;

namespace Convert {
    GRAYSCALE grayscale(const RGB_IMAGE& img);
    LUMINANCE luminance(const RGB_IMAGE& img);
    SEPIA     sepia(const RGB_IMAGE& img);
}
