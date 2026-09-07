#pragma once

#include <Canvas/Image.h>
#include <Canvas/Layer.h>

struct Rect {
    uint32_t x     = 0, y      = 0;
    uint32_t width = 0, height = 0;
};

class Canvas {
public:
    Canvas(uint32_t width, uint32_t height);

    void resize(uint32_t width, uint32_t height);
    void scale(uint32_t width, uint32_t height);

    void rotateLeft();
    void rotateRight();

    void addLayer();
    void addLayer(size_t layerID, Layer layer);
    void deleteLayer(size_t layerID);
    void duplicateLayer(size_t layerID);

    void mergeWithLayerBelow(size_t topLayerID);
    void mergeAllLayers();

    void moveLayerToIndex(size_t layerID, size_t index);

    void updateComposite();
    const Image& getComposite() const noexcept;

    const Layer& operator[](size_t layerID) const;
          Layer& operator[](size_t layerID);

    auto begin()       noexcept { return m_layers.begin(); }
    auto begin() const noexcept { return m_layers.begin(); }
    auto end()         noexcept { return m_layers.end();   }
    auto end()   const noexcept { return m_layers.end();   }

    uint32_t width()      const noexcept;
    uint32_t height()     const noexcept;
    size_t   layerCount() const noexcept;

private:
    std::vector<Layer> m_layers;
    uint32_t           m_width;
    uint32_t           m_height;

    Image              m_composite;
};
