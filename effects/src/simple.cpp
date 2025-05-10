#include "../effects.h"

using Effects::RGB_IMAGE;

void Effects::invert(RGB_IMAGE& img) {
    for (auto& row : img) {
        for (auto& pixel : row) {
            pixel[0] = 255 - pixel[0];
            pixel[1] = 255 - pixel[1];
            pixel[2] = 255 - pixel[2];
        }
    }
}
