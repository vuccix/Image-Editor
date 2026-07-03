#pragma once

#include <cstdint>
#include <vector>

struct Image {
    std::vector<uint8_t> pixels;
    uint32_t             width;
    uint32_t             height;
};
