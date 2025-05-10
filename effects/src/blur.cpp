#include "../effects.h"
#include "../../utils/convolution.h"

// Gaussian blur
void Effects::blur(RGB_IMAGE& img) {
    static constexpr float kernel[3][3] = {
        { 1.f / 16.f, 1.f / 8.f, 1.f / 16.f },
        { 1.f /  8.f, 1.f / 4.f, 1.f /  8.f },
        { 1.f / 16.f, 1.f / 8.f, 1.f / 16.f }
    };
    img = convolution3x3(img, kernel);
}
