#include <Canvas/Canvas.h>
#include <cassert>

Layer::Layer(const int32_t w, const int32_t h, std::string name) : m_width(w), m_height(h) {
    this->name = std::move(name);
    m_data.resize(w * h, Pixel{ 0, 0, 0, 0 });
}

const Pixel& Layer::operator[](const int32_t x, const int32_t y) const {
    assert(y < m_height && y >= 0 && x < m_width && x >= 0);
    return m_data[y * m_width + x];
}

Pixel& Layer::operator[](const int32_t x, const int32_t y) {
    assert(y < m_height && y >= 0 && x < m_width && x >= 0);
    return m_data[y * m_width + x];
}

std::span<const Pixel> Layer::pixels() const { return m_data; }
std::span<Pixel>       Layer::pixels()       { return m_data; }

