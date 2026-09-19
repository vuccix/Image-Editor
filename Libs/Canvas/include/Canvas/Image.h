#pragma once

#include <Canvas/Pixel.h>
#include <cstdint>
#include <vector>

struct Image {
    std::vector<Pixel> pixels;
    uint32_t           width  = 0;
    uint32_t           height = 0;
};
