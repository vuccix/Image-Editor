#pragma once

#include <vector>
#include <array>
#include <cstdint>

using PIXEL     = std::array<uint8_t, 3>;
using RGB_IMAGE = std::vector<std::vector<PIXEL>>;
using GRAYSCALE = std::vector<std::vector<float>>;

GRAYSCALE sobel(const GRAYSCALE& img, bool axis);
GRAYSCALE energyMap(const GRAYSCALE& img);
