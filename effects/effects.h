#pragma once

#include <array>
#include <vector>
#include <cstdint>

using PIXEL     = std::array<uint8_t, 3>;
using RGB_IMAGE = std::vector<std::vector<PIXEL>>;

using GRAYSCALE = std::vector<std::vector<float>>;
using LUMINANCE = GRAYSCALE;

namespace Effects {
    void invert(RGB_IMAGE& img);
    void sepia(RGB_IMAGE& img);
    void seam_carving(RGB_IMAGE& img, int iterCnt, bool isAccurate);
    void blur3x3(RGB_IMAGE& img);
    void blur5x5(RGB_IMAGE& img);
    void channelSwap(RGB_IMAGE& img);
    void brightness(RGB_IMAGE& img, int value);
    void contrast(RGB_IMAGE& img, float factor);
    void duotone(RGB_IMAGE& img, const PIXEL& col1, const PIXEL& col2);
    void pixelate(RGB_IMAGE& img, int size);
    GRAYSCALE grayscale(const RGB_IMAGE& img);
    LUMINANCE luminance(const RGB_IMAGE& img);
    GRAYSCALE sobelOperator(const RGB_IMAGE& img);
    GRAYSCALE cannyEdge(RGB_IMAGE& img);
    RGB_IMAGE rotate90deg(const RGB_IMAGE& img, bool side);
}
