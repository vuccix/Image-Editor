#pragma once

#include <cstdint>
#include <compare>

struct Pixel {
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    uint8_t  operator[](uint32_t channel) const;
    uint8_t& operator[](uint32_t channel);
    auto operator<=>(const Pixel&) const = default;
};
