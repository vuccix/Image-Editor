#pragma once

#include <cstdint>
#include <vector>

struct Pixel {
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;
};

struct Image {
    std::vector<uint8_t> pixels;
    int32_t              width;
    int32_t              height;
    int32_t              channels;
};
