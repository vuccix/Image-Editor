#pragma once
#include <cstdint>

class Layer;
class Canvas;

namespace Effects {

    void flipHorizontally(Layer& image);
    void flipVertically(Layer& image);

    void rotateLeft(Canvas& image);
    void rotateRight(Canvas& image);
    void rotate180(Canvas& image);

    void brightness(Layer& image, int32_t value);
    void contrast(Layer& image, float factor);
    void saturation(Layer& image, float factor);

    void invert(Layer& image);
    void invertAlpha(Layer& image);

    void grayscale(Layer& image);
    void luminance(Layer& image);
    void sepia(Layer& image);

}
