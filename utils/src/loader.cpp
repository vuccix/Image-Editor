#include "../loader.h"
#include "../../effects/effects.h"
#include "../../libs/stb_image.h"
#include "../../libs/stb_image_write.h"
#include <iostream>

bool processImage(const char* input, const char* output, const char* effect) {
    if (!Effects::HAS(effect))
        return false;

    int width, height, channels;
    unsigned char* data = stbi_load(input, &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load image\n";
        return false;
    }

    using namespace Effects;
    RGB_IMAGE image(height, std::vector<std::array<int, 3>>(width));

    for (int y{}; y < height; ++y) {
        for (int x{}; x < width; ++x) {
            int idx = (y * width + x) * channels;
            image[y][x] = { data[idx], data[idx + 1], data[idx + 2] };
        }
    }
    stbi_image_free(data);

    return true;
}
