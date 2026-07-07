#pragma once

class Layer;

namespace Effects {

    void brightness(Layer& image);
    void contrast(Layer& image);
    void saturation(Layer& image);

    void invert(Layer& image);
    void invertAlpha(Layer& image);

    void grayscale(Layer& image);
    void luminance(Layer& image);
    void sepia(Layer& image);

}
