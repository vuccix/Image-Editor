#pragma once

class Layer;
class Canvas;

namespace Filters {

    void duoTone(Layer& image);
    void waterColor(Layer& image);
    void oilPainting(Layer& image);

    void blur(Layer& image);
    void gaussianBlur(Layer& image);
    void motionBlur(Layer& image);

    void swapChannels(Layer& image, int change);
    void emboss(Layer& image);
    void outline(Layer& image);
    void sharpen(Layer& image);
    void pixelate(Layer& image);

    void canny(Layer& image);
    void laplace(Layer& image);
    void prewitt(Layer& image);
    void scharr(Layer& image);
    void sobel(Layer& image);

    void fourierTransform(Canvas& image);
    void normalMap(Layer& image);
    void seamCarving(Canvas& image);

}
