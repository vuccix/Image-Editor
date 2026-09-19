#include <Canvas/Canvas.h>
#include <cassert>
#include <cstring>
#include <format>
#include <omp.h>

Canvas::Canvas(const uint32_t w, const uint32_t h) : m_width(w), m_height(h) {
    assert(w > 0 && h > 0);
    m_layers.emplace_back(m_width, m_height, "Background");
}

void Canvas::resize(const uint32_t w, const uint32_t h) {
    assert(w > 0 && h > 0);

    if (m_width == w && m_height == h)
        return;

    m_width  = w;
    m_height = h;

    for (Layer& layer : m_layers)
        layer.resize(w, h);
}

void Canvas::scale(const uint32_t w, const uint32_t h) {
    assert(w > 0 && h > 0);

    if (m_width == w && m_height == h)
        return;

    m_width  = w;
    m_height = h;

    for (Layer& layer : m_layers)
        layer.scale(w, h);
}

namespace {

std::vector<Pixel> rotate(Layer& layer, auto&& mapCoords) {
    std::vector result(layer.width() * layer.height(), Pixel{});

    const std::mdspan newPixels(result.data(), layer.width(), layer.height());
    const std::mdspan pixels = layer.pixels();

    #pragma omp parallel for collapse(2)
    for (uint32_t y = 0; y < layer.height(); ++y) {
        for (uint32_t x = 0; x < layer.width(); ++x) {
            const auto [targetX, targetY] = mapCoords(x, y, layer.width(), layer.height());
            newPixels[targetX, targetY]   = pixels[y, x];
        }
    }

    return result;
}

}

void Canvas::rotateLeft() {
    auto mapCoords = [](const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h) {
        return std::make_pair(w - 1 - x, y);
    };

    for (Layer& layer : m_layers) {
        std::vector<Pixel> data = ::rotate(layer, mapCoords);
        layer.setData(std::move(data), layer.height(), layer.width());
    }

    std::swap(m_width, m_height);
}

void Canvas::rotateRight() {
    auto mapCoords = [](const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h) {
        return std::make_pair(x, h - 1 - y);
    };

    for (Layer& layer : m_layers) {
        std::vector<Pixel> data = ::rotate(layer, mapCoords);
        layer.setData(std::move(data), layer.height(), layer.width());
    }

    std::swap(m_width, m_height);
}

void Canvas::addLayer() {
    m_layers.emplace_back(m_width, m_height, std::format("Layer {}", m_layers.size()));
}

void Canvas::addLayer(const size_t layerID) {
    Layer layer(m_width, m_height, std::format("Layer {}", m_layers.size()));
    addLayer(layerID, std::move(layer));
}

void Canvas::addLayer(const size_t layerID, Layer layer) {
    m_layers.emplace(m_layers.begin() + static_cast<int64_t>(layerID), std::move(layer));
}

void Canvas::deleteLayer(const size_t layerID) {
    assert(layerID < m_layers.size());
    m_layers.erase(m_layers.begin() + layerID);
}

void Canvas::duplicateLayer(const size_t layerID) {
    assert(layerID < m_layers.size());
    const auto layer = m_layers.insert(m_layers.begin() + layerID + 1, m_layers[layerID]);
    layer->name      = layer->name + " (copy)";
}

namespace {

Pixel mergePixels(const Pixel bot, const Pixel top, const float opacity, const float fill) {
    constexpr float norm = 1.f / 255.f;

    const     float botR = bot.r * norm;
    const     float botG = bot.g * norm;
    const     float botB = bot.b * norm;
    const     float botA = bot.a * norm;

    const     float topR = top.r * norm;
    const     float topG = top.g * norm;
    const     float topB = top.b * norm;
    const     float topA = top.a * norm * opacity * fill;

    const     float outA = topA + botA * (1.f - topA);
    const     float invA = outA > 0.f  ? (1.f / outA) : 0.f;

    const     float outR = (topR * topA + botR * botA * (1.f - topA)) * invA;
    const     float outG = (topG * topA + botG * botA * (1.f - topA)) * invA;
    const     float outB = (topB * topA + botB * botA * (1.f - topA)) * invA;

    return Pixel{
        .r = static_cast<uint8_t>(std::clamp(outR * 255.f, 0.f, 255.f)),
        .g = static_cast<uint8_t>(std::clamp(outG * 255.f, 0.f, 255.f)),
        .b = static_cast<uint8_t>(std::clamp(outB * 255.f, 0.f, 255.f)),
        .a = static_cast<uint8_t>(std::clamp(outA * 255.f, 0.f, 255.f))
    };
}

}

void Canvas::mergeWithLayerBelow(const size_t topLayerID) {
    assert(topLayerID > 0 && topLayerID < m_layers.size());

    const auto top      = m_layers[topLayerID].pixels();
    const auto bottom   = m_layers[topLayerID - 1].pixels();

    const float opacity = m_layers[topLayerID].opacity;
    const float fill    = m_layers[topLayerID].fill;

    for (uint32_t y = 0; y < m_height; ++y) {
        for (uint32_t x = 0; x < m_width; ++x) {
            const Pixel b = bottom[y, x];
            const Pixel t = top[y, x];
            bottom[y, x]  = ::mergePixels(b, t, opacity, fill);
        }
    }

    m_layers[topLayerID - 1].name = std::move(m_layers[topLayerID].name);
    m_layers.erase(m_layers.begin() + topLayerID);
}

void Canvas::mergeAllLayers() {
    while (m_layers.size() > 1)
        mergeWithLayerBelow(m_layers.size() - 1);
}

void Canvas::moveLayerToIndex(const size_t layerID, const size_t index) {
    assert(layerID < m_layers.size() && index < m_layers.size());

    const auto lID = static_cast<int64_t>(layerID);
    const auto idx = static_cast<int64_t>(index);

    // move up
    if (layerID < index) {
        std::rotate(m_layers.begin() + lID,
                    m_layers.begin() + lID + 1,
                    m_layers.begin() + idx   + 1);
    }
    // move down
    else {
        std::rotate(m_layers.begin() + idx,
                    m_layers.begin() + lID,
                    m_layers.begin() + lID + 1);
    }
}

void Canvas::replaceWithImage(Image image) {
    assert(image.width > 0 && image.height > 0);
    assert(image.pixels.empty() == false);

    std::vector<Pixel> pixels(image.pixels.size() / sizeof(Pixel));
    std::memcpy(
        pixels.data(),
        image.pixels.data(),
        image.pixels.size()
    );

    Layer layer(image.width, image.height, "Background");
    layer.setData(std::move(pixels));

    m_layers.clear();
    m_layers.emplace_back(std::move(layer));

    m_width     = image.width;
    m_height    = image.height;
    m_composite = std::move(image);
}

void Canvas::updateComposite() {
    assert(m_layers.empty() == false && "Canvas has no layers!");
    assert(m_width > 0 && m_height > 0 && "Incorrect width and/or height!");

    if (m_composite.width != m_width || m_composite.height != m_height) {
        m_composite.width  = m_width;
        m_composite.height = m_height;
        m_composite.pixels.resize(m_width * m_height * sizeof(Pixel));
    }

    auto mergePixelStack = [&](const uint32_t x, const uint32_t y) -> Pixel {
        Pixel res{ 0, 0, 0, 0 };

        for (const Layer& layer : m_layers) {
            // skip hidden layers
            if (!layer.isActive)
                continue;

            res = ::mergePixels(res, layer.pixels()[y, x], layer.opacity, layer.fill);
        }

        return res;
    };

    uint8_t* outPtr = m_composite.pixels.data();
    for (uint32_t y = 0; y < m_height; ++y) {
        for (uint32_t x = 0; x < m_width; ++x) {
            const auto [r, g, b, a] = mergePixelStack(x, y);
            *outPtr++ = r;
            *outPtr++ = g;
            *outPtr++ = b;
            *outPtr++ = a;
        }
    }
}

const Image& Canvas::getComposite() const noexcept {
    return m_composite;
}

const Layer& Canvas::operator[](const size_t layerID) const { return m_layers[layerID]; }
      Layer& Canvas::operator[](const size_t layerID)       { return m_layers[layerID]; }

uint32_t Canvas::width()      const noexcept { return m_width;         }
uint32_t Canvas::height()     const noexcept { return m_height;        }
size_t   Canvas::layerCount() const noexcept { return m_layers.size(); }
