#include "../loader.h"
#include "../../effects/effects.h"
#include "../../libs/stb_image.h"
#include "../../libs/stb_image_write.h"
#include <iostream>
#include <cstring>

bool processImage(const char* input, const char* output, const char* effect) {
    // check correct input
//    if (!Effects::HAS(effect)) {
//    if (!Effects::AVAILABLE_EFFECTS.contains(effect)) {
//        std::cerr << "Effect: \"" << effect << "\" not available\n";
//        return false;
//    }

    // load given image
    int width, height, channels;
    unsigned char* data = stbi_load(input, &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load image\n";
        return false;
    }

    using Effects::RGB_IMAGE, Effects::GRAYSCALE;
    RGB_IMAGE image(height, std::vector<std::array<uint8_t , 3>>(width));

    for (int y{}; y < height; ++y)
        for (int x{}; x < width; ++x) {
            int idx = (y * width + x) * channels;
            image[y][x] = { data[idx], data[idx + 1], data[idx + 2] };
        }

    stbi_image_free(data);

    if (strcmp(effect, "grayscale") == 0) {
        GRAYSCALE gray = Effects::grayscale(image);
    }
    else if (strcmp(effect, "luminance") == 0) {
        GRAYSCALE gray = Effects::luminance(image);
    }
    else if (strcmp(effect, "sepia") == 0) {
        Effects::sepia(image);
    }
    else if (strcmp(effect, "invert") == 0) {
        Effects::invert(image);
    }
    else if (strcmp(effect, "seam_carving") == 0) {
        int cnt{};
        std::cout << "Number of iterations: ";
        std::cin >> cnt;

        if (std::cin.fail() || cnt < 1) {
            std::cerr << "Incorrect input!\n";
            return false;
        }

        std::cout << "start\n";
        image = Effects::seam_carving(image, cnt, false);
        std::cout << "finish\n";
        width -= cnt;
    }

    std::vector<unsigned char> output_data(width * height * 3);
    for (int y{}; y < height; ++y) {
        for (int x{}; x < width; ++x) {
            int idx = (y * width + x) * 3;
            output_data[idx + 0] = image[y][x][0];
            output_data[idx + 1] = image[y][x][1];
            output_data[idx + 2] = image[y][x][2];
        }
    }

    stbi_write_png(output, width, height, 3, output_data.data(), width * 3);

    return true;
}
