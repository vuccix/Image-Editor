#pragma once

#include <cstdint>

class Layer;
class Canvas;

namespace Effects {

    void flipHorizontally(Layer& image);
    void flipVertically(Layer& image);
    void flipHorizontallyCanvas(Canvas& canvas);
    void flipVerticallyCanvas(Canvas& canvas);

    void rotate180(Canvas& canvas);

    void brightness(Layer& image, int32_t value);
    void contrast(Layer& image, float factor);
    void saturation(Layer& image, float factor);

    void invert(Layer& image);
    void invertAlpha(Layer& image);

    void grayscale(Layer& image);
    void luminance(Layer& image);
    void sepia(Layer& image);

}
