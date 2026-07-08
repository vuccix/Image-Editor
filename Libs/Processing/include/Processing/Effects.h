#pragma once
#include "Canvas/Canvas.h"

class Layer;
class Canvas;

namespace Effects {

    void flipHorizontally(Layer& image);
    void flipVertically(Layer& image);
    void flipHorizontally(Canvas& canvas);
    void flipVertically(Canvas& canvas);

    void rotateLeft(Canvas& canvas);
    void rotateRight(Canvas& canvas);
    void rotate180(Canvas& canvas);

    void brightness(Layer& image, int value);
    void contrast(Layer& image, float factor);
    void saturation(Layer& image, float factor);

    void invert(Layer& image);
    void invertAlpha(Layer& image);

    void grayscale(Layer& image);
    void luminance(Layer& image);
    void sepia(Layer& image);

}
