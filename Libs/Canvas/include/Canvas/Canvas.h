#pragma once

#include <Canvas/Image.h>
#include <string>
#include <mdspan>

class Layer {
public:
    Layer(int32_t width, int32_t height, std::string name);

    void resize(int32_t width, int32_t height);
    void scale(int32_t width, int32_t height);

    const Pixel& operator[](int32_t x, int32_t y) const;
          Pixel& operator[](int32_t x, int32_t y);

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

class Canvas {
public:
    Canvas(int32_t width, int32_t height);

    void resize(int32_t width, int32_t height);
    void scale(int32_t width, int32_t height);

    void addLayer();
    void deleteLayer(size_t layerID);
    void duplicateLayer(size_t layerID);

    void mergeWithLayerBelow(size_t layerID);
    void mergeAllLayers();

    void moveLayerToIndex(size_t layerID, size_t index);

    Image getComposite() const;

    const Layer& operator[](size_t layerID) const;
          Layer& operator[](size_t layerID);

    int32_t width()  const;
    int32_t height() const;

private:
    std::vector<Layer> m_layers;
    int32_t            m_width;
    int32_t            m_height;
};
