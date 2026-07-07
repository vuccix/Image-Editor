#pragma once

class Layer;

namespace Effects {

    void flipHorizontally(Layer& image);
    void flipVertically(Layer& image);

    void rotateLeft(Layer& image);
    void rotateRight(Layer& image);
    void rotate180(Layer& image);

    void brightness(Layer& image);
    void contrast(Layer& image);
    void saturation(Layer& image);

    void invert(Layer& image);
    void invertAlpha(Layer& image);

    void grayscale(Layer& image);
    void luminance(Layer& image);
    void sepia(Layer& image);

}
