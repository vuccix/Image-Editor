#pragma once

#include <Canvas/Image.h>
#include <Canvas/Layer.h>

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

    void updateComposite();
    const Image& getComposite();

    const Layer& operator[](size_t layerID) const;
          Layer& operator[](size_t layerID);

    int32_t width()  const;
    int32_t height() const;

private:
    std::vector<Layer> m_layers;
    int32_t            m_width;
    int32_t            m_height;

    Image              m_composite;
};
