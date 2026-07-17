#pragma once

#include <Canvas/Pixel.h>
#include <vector>
#include <string>
#include <mdspan>
#include <span>

class Layer {
public:
    Layer(uint32_t width, uint32_t height, std::string name);

    void resize(uint32_t width, uint32_t height);
    void scale(uint32_t width, uint32_t height);

    std::mdspan<const Pixel, std::dextents<size_t, 2>> pixels() const;
    std::mdspan<Pixel,       std::dextents<size_t, 2>> pixels();

    std::span<const Pixel> data() const;
    std::span<Pixel>       data();

    void setData(std::vector<Pixel>&& data, uint32_t width = 0, uint32_t height = 0);

    uint32_t width()  const;
    uint32_t height() const;

public:
    std::string        name;
    bool               isActive = true;
    float              opacity  = 1.f; // affects entire layer (styles, effects, blend modes)
    float              fill     = 1.f; // affects only pixels

private:
    std::vector<Pixel> m_data;
    uint32_t           m_width;
    uint32_t           m_height;
};
