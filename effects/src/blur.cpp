#include "../effects.h"
#include "../../utils/convolution.h"

// Gaussian blur
void Effects::blur3x3(RGB_IMAGE& img) {
    static constexpr float kernel[3][3] = {
        { 1.f / 16.f, 1.f / 8.f, 1.f / 16.f },
        { 1.f /  8.f, 1.f / 4.f, 1.f /  8.f },
        { 1.f / 16.f, 1.f / 8.f, 1.f / 16.f }
    };
    img = convolution3x3(img, kernel);
}

void Effects::blur5x5(RGB_IMAGE& img) {
    static constexpr float kernel[5][5] = {
        { 2.f / 159.f,  4.f / 159.f,  5.f / 159.f,  4.f / 159.f, 2.f / 159.f },
        { 4.f / 159.f,  9.f / 159.f, 12.f / 159.f,  9.f / 159.f, 9.f / 159.f },
        { 5.f / 159.f, 12.f / 159.f, 15.f / 159.f, 12.f / 159.f, 5.f / 159.f },
        { 4.f / 159.f,  9.f / 159.f, 12.f / 159.f,  9.f / 159.f, 9.f / 159.f },
        { 2.f / 159.f,  4.f / 159.f,  5.f / 159.f,  4.f / 159.f, 2.f / 159.f }
    };
    img = convolution5x5(img, kernel);
}
