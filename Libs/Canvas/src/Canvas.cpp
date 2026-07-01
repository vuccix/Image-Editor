#include <Canvas/Canvas.h>
#include <algorithm>
#include <cmath>
#include <cassert>

Layer::Layer(const int32_t w, const int32_t h, std::string name) : m_width(w), m_height(h) {
    this->name = std::move(name);
    m_data.resize(w * h, Pixel{ 0, 0, 0, 0 });
}

void Layer::resize(const int32_t w, const int32_t h) {
    assert(w >= 1 && h >= 1);

    std::vector newData(w * h, Pixel{ 0, 0, 0, 0 });
    const auto  oldSpan    = std::mdspan(m_data.data(), m_height, m_width);
    const auto  newSpan    = std::mdspan(newData.data(), h, w);

    const int32_t w_offset = (w - m_width)  / 2;
    const int32_t h_offset = (h - m_height) / 2;

    for (int32_t y = 0; y < m_height; ++y) {
        for (int32_t x = 0; x < m_width; ++x) {
            const int32_t newX = x + w_offset;
            const int32_t newY = y + h_offset;

            if (newX >= 0 && newX < w && newY >= 0 && newY < h)
                newSpan[newY, newX] = oldSpan[y, x];
        }
    }

    m_width  = w;
    m_height = h;
    m_data   = std::move(newData);
}

void Layer::scale(const int32_t w, const int32_t h) {
    assert(w >= 1 && h >= 1);

    std::vector newData(w * h, Pixel{ 0, 0, 0, 0 });
    const auto  oldSpan = std::mdspan(m_data.data(), m_height, m_width);
    const auto  newSpan = std::mdspan(newData.data(), h, w);

    const double scaleX = static_cast<double>(m_width)  / w;
    const double scaleY = static_cast<double>(m_height) / h;

    for (int32_t y = 0; y < h; ++y) {
        for (int32_t x = 0; x < w; ++x) {
            const double xFactor = x * scaleX;
            const double yFactor = y * scaleY;

            const int32_t oldX   = std::clamp(static_cast<int32_t>(std::floor(xFactor)), 0, m_width  - 1);
            const int32_t oldY   = std::clamp(static_cast<int32_t>(std::floor(yFactor)), 0, m_height - 1);

            newSpan[y, x]        = oldSpan[oldY, oldX];
        }
    }

    m_width  = w;
    m_height = h;
    m_data   = std::move(newData);
}

const Pixel& Layer::operator[](const int32_t x, const int32_t y) const {
    assert(y < m_height && y >= 0 && x < m_width && x >= 0);
    return m_data[y * m_width + x];
}

Pixel& Layer::operator[](const int32_t x, const int32_t y) {
    assert(y < m_height && y >= 0 && x < m_width && x >= 0);
    return m_data[y * m_width + x];
}

std::mdspan<const Pixel, std::dextents<size_t, 2>> Layer::pixels() const {
    return std::mdspan(m_data.data(), m_height, m_width);
}

std::mdspan<Pixel, std::dextents<size_t, 2>> Layer::pixels() {
    return std::mdspan(m_data.data(), m_height, m_width);
}

