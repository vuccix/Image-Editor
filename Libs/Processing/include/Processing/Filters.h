#pragma once

class Layer;
class Canvas;

namespace Filters {

    void duoTone(Layer& image, const float colorA[3], const float colorB[3]);
    void waterColor(Layer& image);
    void oilPainting(Layer& image);

    void blur(Layer& image, int amount);
    void gaussianBlur(Layer& image, int amount);
    void motionBlur(Layer& image, int distance, float angle);

    void swapChannels(Layer& image, int change);
    void emboss(Layer& image);
    void outline(Layer& image);
    void sharpen(Layer& image, float amount);
    void pixelate(Layer& image, int blockSize);

    void canny(Layer& image, float lowThreshold, float highThreshold);
    void laplace(Layer& image);
    void prewitt(Layer& image);
    void scharr(Layer& image);
    void sobel(Layer& image);

    void fourierTransform(Canvas& image);
    void normalMap(Layer& image, float strength, bool flipY);
    void seamCarving(Canvas& image);

}
