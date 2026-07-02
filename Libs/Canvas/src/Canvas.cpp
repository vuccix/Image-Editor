#include <Canvas/Canvas.h>
#include <cassert>
#include <format>

Canvas::Canvas(const int32_t w, const int32_t h) : m_width(w), m_height(h) {
    m_layers.emplace_back(m_width, m_height, "Background");
}

void Canvas::resize(const int32_t w, const int32_t h) {
    m_width  = w;
    m_height = h;

    for (auto& layer : m_layers)
        layer.resize(w, h);
}

void Canvas::scale(const int32_t w, const int32_t h) {
    m_width  = w;
    m_height = h;

    for (auto& layer : m_layers)
        layer.scale(w, h);
}

void Canvas::addLayer() {
    m_layers.emplace_back(m_width, m_height, std::format("Layer {}", m_layers.size()));
}

void Canvas::deleteLayer(const size_t layerID) {
    assert(layerID < m_layers.size());
    m_layers.erase(m_layers.begin() + layerID);
}

void Canvas::duplicateLayer(const size_t layerID) {
    assert(layerID < m_layers.size());
    m_layers.insert(m_layers.begin() + layerID + 1, m_layers[layerID]);
}

namespace {

Pixel mergePixels(const Pixel bot, const Pixel top, const float opacity, const float fill) {
    const float botR =  bot.r / 255.f;
    const float botG =  bot.g / 255.f;
    const float botB =  bot.b / 255.f;
    const float botA =  bot.a / 255.f;

    const float topR =  top.r / 255.f;
    const float topG =  top.g / 255.f;
    const float topB =  top.b / 255.f;
    const float topA = (top.a / 255.f) * opacity * fill;

    const float outA = topA + botA  * (1.f - topA);
    const float invA = (outA > 0.f) ? (1.f / outA) : 0.f;

    const float outR = (topR * topA + botR * botA * (1.f - topA)) * invA;
    const float outG = (topG * topA + botG * botA * (1.f - topA)) * invA;
    const float outB = (topB * topA + botB * botA * (1.f - topA)) * invA;

    return Pixel{
        static_cast<std::uint8_t>(std::clamp(outR * 255.f, 0.f, 255.f)),
        static_cast<std::uint8_t>(std::clamp(outG * 255.f, 0.f, 255.f)),
        static_cast<std::uint8_t>(std::clamp(outB * 255.f, 0.f, 255.f)),
        static_cast<std::uint8_t>(std::clamp(outA * 255.f, 0.f, 255.f))
    };
}

}

void Canvas::mergeWithLayerBelow(const size_t layerID) {
    assert(layerID > 0 && layerID < m_layers.size());

    const auto top      = m_layers[layerID].pixels();
    const auto bottom   = m_layers[layerID - 1].pixels();

    const float opacity = m_layers[layerID].opacity;
    const float fill    = m_layers[layerID].fill;

    for (int32_t y = 0; y < m_height; ++y) {
        for (int32_t x = 0; x < m_width; ++x) {
            const Pixel b = bottom[y, x];
            const Pixel t = top[y, x];
            bottom[y, x]  = ::mergePixels(b, t, opacity, fill);
        }
    }

    m_layers.erase(m_layers.begin() + layerID);
}

void Canvas::mergeAllLayers() {
    while (m_layers.size() > 1)
        mergeWithLayerBelow(m_layers.size() - 1);
}

void Canvas::moveLayerToIndex(const size_t layerID, const size_t index) {
    assert(layerID < m_layers.size() && index < m_layers.size());

    // move up
    if (layerID < index) {
        std::rotate(m_layers.begin() + layerID,
                    m_layers.begin() + layerID + 1,
                    m_layers.begin() + index   + 1);
    }
    // move down
    else {
        std::rotate(m_layers.begin() + index,
                    m_layers.begin() + layerID,
                    m_layers.begin() + layerID + 1);
    }
}

void Canvas::updateComposite() {
    if (m_composite.width != m_width || m_composite.height != m_height) {
        m_composite.width  = m_width;
        m_composite.height = m_height;
        m_composite.pixels.resize(m_width * m_height * sizeof(Pixel));
    }

    const size_t numLayers = m_layers.size();
    if (numLayers == 0) {
        std::ranges::fill(m_composite.pixels, 0);
        return;
    }

    auto mergePixelStack = [&](const int32_t x, const int32_t y) -> Pixel {
        Pixel res = m_layers[0].pixels()[y, x];

        for (size_t i = 1; i < numLayers; ++i) {
            const auto& layer = m_layers[i];

            // skip hidden layers
            if (!layer.isActive)
                continue;

            res = ::mergePixels(res, layer.pixels()[y, x], layer.opacity, layer.fill);
        }

        return res;
    };

    uint8_t* outPtr = m_composite.pixels.data();
    for (int32_t y = 0; y < m_height; ++y) {
        for (int32_t x = 0; x < m_width; ++x) {
            const auto [r, g, b, a] = mergePixelStack(x, y);
            *outPtr++ = r;
            *outPtr++ = g;
            *outPtr++ = b;
            *outPtr++ = a;
        }
    }
}

const Image& Canvas::getComposite() {
    return m_composite;
}

const Layer& Canvas::operator[](const size_t layerID) const { return m_layers[layerID]; }
      Layer& Canvas::operator[](const size_t layerID)       { return m_layers[layerID]; }

int32_t Canvas::width()  const { return m_width;  }
int32_t Canvas::height() const { return m_height; }
