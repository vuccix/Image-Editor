#pragma once

#include <vector>
#include <array>
#include <cstdint>

using PIXEL     = std::array<uint8_t, 3>;
using RGB_IMAGE = std::vector<std::vector<PIXEL>>;
using GRAYSCALE = std::vector<std::vector<float>>;

RGB_IMAGE convolution3x3(const RGB_IMAGE& img, const float kernel[3][3]);
GRAYSCALE convolution3x3(const GRAYSCALE& img, const float kernel[3][3]);
RGB_IMAGE convolution5x5(const RGB_IMAGE& img, const float kernel[5][5]);
GRAYSCALE convolution5x5(const GRAYSCALE& img, const float kernel[5][5]);
