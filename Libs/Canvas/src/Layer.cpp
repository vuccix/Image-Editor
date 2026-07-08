#include <Canvas/Layer.h>
#include <algorithm>
#include <cassert>
#include <cmath>

uint8_t Pixel::operator[](const uint32_t channel) const {
    assert(channel <= 3);

    switch (channel) {
        case 0:  return r;
        case 1:  return g;
        case 2:  return b;
        default: return a;
    }
}

Layer::Layer(const uint32_t w, const uint32_t h, std::string n) : name(std::move(n)),
                                                                  m_width(w),
                                                                  m_height(h) {
    m_data.resize(w * h, Pixel{ 0, 0, 0, 0 });
}

void Layer::resize(const uint32_t w, const uint32_t h) {
    assert(w >= 1 && h >= 1);

    if (m_width == w && m_height == h)
        return;

    std::vector newData(w * h, Pixel{ 0, 0, 0, 0 });
    const auto  oldSpan   = std::mdspan(m_data.data(), m_height, m_width);
    const auto  newSpan   = std::mdspan(newData.data(), h, w);

    const int32_t wOffset = (static_cast<int32_t>(w) - static_cast<int32_t>(m_width))  / 2;
    const int32_t hOffset = (static_cast<int32_t>(h) - static_cast<int32_t>(m_height)) / 2;

    for (uint32_t y = 0; y < m_height; ++y) {
        const int32_t newY = static_cast<int32_t>(y) + hOffset;

        for (uint32_t x = 0; x < m_width; ++x) {
            const int32_t newX = static_cast<int32_t>(x) + wOffset;

            if (newX >= 0 && static_cast<uint32_t>(newX) < w && newY >= 0 && static_cast<uint32_t>(newY) < h)
                newSpan[newY, newX] = oldSpan[y, x];
        }
    }

    m_width  = w;
    m_height = h;
    m_data   = std::move(newData);
}

void Layer::scale(const uint32_t w, const uint32_t h) {
    assert(w >= 1 && h >= 1);

    if (m_width == w && m_height == h)
        return;

    std::vector newData(w * h, Pixel{ 0, 0, 0, 0 });
    const auto  oldSpan = std::mdspan(m_data.data(), m_height, m_width);
    const auto  newSpan = std::mdspan(newData.data(), h, w);

    const double scaleX = static_cast<double>(m_width)  / w;
    const double scaleY = static_cast<double>(m_height) / h;

    for (uint32_t y = 0; y < h; ++y) {
        const double yFactor = y * scaleY;

        for (uint32_t x = 0; x < w; ++x) {
            const double xFactor = x * scaleX;

            const uint32_t oldX  = std::clamp(static_cast<uint32_t>(std::floor(xFactor)), 0u, m_width  - 1);
            const uint32_t oldY  = std::clamp(static_cast<uint32_t>(std::floor(yFactor)), 0u, m_height - 1);

            newSpan[y, x]        = oldSpan[oldY, oldX];
        }
    }

    m_width  = w;
    m_height = h;
    m_data   = std::move(newData);
}

std::mdspan<const Pixel, std::dextents<size_t, 2>> Layer::pixels() const {
    return std::mdspan(m_data.data(), m_height, m_width);
}

std::mdspan<Pixel, std::dextents<size_t, 2>> Layer::pixels() {
    return std::mdspan(m_data.data(), m_height, m_width);
}

std::span<const Pixel> Layer::data() const { return m_data; }
std::span<Pixel>       Layer::data()       { return m_data; }

void Layer::setData(std::vector<Pixel>&& data, const uint32_t w, const uint32_t h) {
    assert((w >= 1 && h >= 1) || (w == 0 && h == 0));

    if (w != 0 || h != 0) {
        m_width  = w;
        m_height = h;
    }

    m_data = std::move(data);
}

std::vector<Pixel> Layer::copyData() const { return m_data;   }
uint32_t           Layer::width()    const { return m_width;  }
uint32_t           Layer::height()   const { return m_height; }
