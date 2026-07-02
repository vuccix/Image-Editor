#pragma once

#include <cstdint>
#include <vector>

struct Image {
    std::vector<uint8_t> pixels;
    int32_t              width;
    int32_t              height;
};
