#pragma once

#include <cstdint>

class Layer;
class Canvas;

namespace Filters {

    void duoTone(Layer& image, const float colorA[3], const float colorB[3]);
    void waterColor(Layer& image);
    void oilPainting(Layer& image);

    void blur(Layer& image, int32_t amount);
    void gaussianBlur(Layer& image, int32_t amount);
    void motionBlur(Layer& image, int32_t distance, float angle);

    void swapChannels(Layer& image, int32_t change);
    void emboss(Layer& image);
    void outline(Layer& image);
    void sharpen(Layer& image, float amount);
    void pixelate(Layer& image, int32_t blockSize);

    void canny(Layer& image, float lowThreshold, float highThreshold);
    void laplace(Layer& image);
    void prewitt(Layer& image);
    void scharr(Layer& image);
    void sobel(Layer& image);

    void fourierTransform(Canvas& image);
    void normalMap(Layer& image, float strength, bool flipY);
    void seamCarving(Canvas& canvas, uint32_t width, uint32_t height);

}
