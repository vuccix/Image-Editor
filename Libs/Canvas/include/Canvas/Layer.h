#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <mdspan>

struct Pixel {
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    uint8_t operator[](int32_t channel) const;
};

class Layer {
public:
    Layer(int32_t width, int32_t height, std::string name);

    void resize(int32_t width, int32_t height);
    void scale(int32_t width, int32_t height);

    std::mdspan<const Pixel, std::dextents<size_t, 2>> pixels() const;
    std::mdspan<Pixel,       std::dextents<size_t, 2>> pixels();

public:
    std::string        name;
    bool               isActive = true;
    float              opacity  = 1.f; // affects entire layer (styles, effects, blend modes)
    float              fill     = 1.f; // affects only pixels

private:
    std::vector<Pixel> m_data;
    int32_t            m_width;
    int32_t            m_height;
};
