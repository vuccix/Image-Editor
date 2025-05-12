#include "../loader.h"
#include "../../libs/stb_image.h"
#include "../../libs/stb_image_write.h"
#include <iostream>
#include <cstring>

void grayscaleWrite(int width, int height, const GRAYSCALE& image, const char* output) {
    std::vector<unsigned char> grayData(width * height);

    for (int y{}; y < height; ++y)
        for (int x{}; x < width; ++x) {
            int gray = static_cast<int>((image[y][x]));
            grayData[y * width + x] = static_cast<unsigned char>(std::min(255, gray));
        }

    stbi_write_png(output, width, height, 1, grayData.data(), width);
}

void rgbWrite(int width, int height, const RGB_IMAGE& image, const char* output) {
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
}

bool processImage(const char* input, const char* output, const char* effect) {
    // load given image
    int width, height, channels;
    unsigned char* data = stbi_load(input, &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load image\n";
        return false;
    }

    RGB_IMAGE image(height, std::vector<PIXEL>(width));

    for (int y{}; y < height; ++y)
        for (int x{}; x < width; ++x) {
            int idx = (y * width + x) * channels;
            image[y][x] = { data[idx], data[idx + 1], data[idx + 2] };
        }

    stbi_image_free(data);

    if (strcmp(effect, "grayscale") == 0) {
        GRAYSCALE gray = Effects::grayscale(image);
        grayscaleWrite(width, height, gray, output);
        return true;
    }
    else if (strcmp(effect, "luminance") == 0) {
        GRAYSCALE gray = Effects::luminance(image);
        grayscaleWrite(width, height, gray, output);
        return true;
    }
    else if (strcmp(effect, "sepia") == 0) {
        Effects::sepia(image);
    }
    else if (strcmp(effect, "invert") == 0) {
        Effects::invert(image);
    }
    else if (strcmp(effect, "blur3") == 0) {
        Effects::blur3x3(image);
    }
    else if (strcmp(effect, "blur5") == 0) {
        Effects::blur5x5(image);
    }
    else if (strcmp(effect, "swap") == 0) {
        Effects::channelSwap(image);
    }
    else if (strcmp(effect, "pixel") == 0) {
        int value{};
        std::cout << "Block size: ";
        std::cin  >> value;

        if (std::cin.fail()) {
            std::cerr << "Incorrect input!\n";
            return false;
        }

        Effects::pixelate(image, value);
    }
    else if (strcmp(effect, "duotone") == 0) {
        PIXEL high, low;

        int r1, g1, b1, r2, g2, b2;

        std::cout << "Enter 6 RGB values (0–255) for high and low colors: ";
        std::cin >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
        if (std::cin.fail()) {
            std::cerr << "Incorrect input!\n";
            return false;
        }

        auto inRange = [](int val) { return val <= 255 && val >= 0; };
        if (!inRange(r1) || !inRange(g1) || !inRange(b1) ||
            !inRange(r2) || !inRange(g2) || !inRange(b2)) {
            std::cerr << "Incorrect input!\n";
            return false;
        }

        high = { uint8_t(r1), uint8_t(g1), uint8_t(b1) };
        low  = { uint8_t(r2), uint8_t(g2), uint8_t(b2) };

        Effects::duotone(image, high, low);
    }
    else if (strcmp(effect, "brightness") == 0) {
        int value{};
        std::cout << "Brightness value: ";
        std::cin  >> value;

        if (std::cin.fail()) {
            std::cerr << "Incorrect input!\n";
            return false;
        }

        Effects::brightness(image, value);
    }
    else if (strcmp(effect, "contrast") == 0) {
        float factor{};
        std::cout << "Contrast factor: ";
        std::cin >> factor;

        if (std::cin.fail()) {
            std::cerr << "Incorrect input!\n";
            return false;
        }

        Effects::contrast(image, factor);
    }
    else if (strcmp(effect, "sobel") == 0) {
        GRAYSCALE gray = Effects::sobelOperator(image);
        grayscaleWrite(width, height, gray, output);
        return true;
    }
    else if (strcmp(effect, "canny") == 0) {
        GRAYSCALE gray = Effects::cannyEdge(image);
        grayscaleWrite(width, height, gray, output);
        return true;
    }
    else if (strcmp(effect, "seam_carving") == 0) {
        int cntW, cntH;

        std::cout << "Number of iterations (width): ";
        std::cin >> cntW;
        if (std::cin.fail() || cntW < 0 || cntW >= width) {
            std::cerr << "Incorrect input!\n";
            return false;
        }

        std::cout << "Number of iterations (height): ";
        std::cin >> cntH;
        if (std::cin.fail() || cntH < 0 || cntH >= height) {
            std::cerr << "Incorrect input!\n";
            return false;
        }

        if (cntW != 0) {
            Effects::seam_carving(image, cntW, false);
            width -= cntW;
        }

        if (cntH != 0) {
            image = Effects::rotate90deg(image, true);
            Effects::seam_carving(image, cntH, false);
            image = Effects::rotate90deg(image, false);
            height -= cntH;
        }
    }
    else if (strcmp(effect, "rotate") == 0) {
        char sign;
        std::cout << "Left or right (l/r): ";
        std::cin >> sign;

        if (std::cin.fail() || (sign != 'l' && sign != 'r')) {
            std::cerr << "Incorrect input!\n";
            return false;
        }

        image = Effects::rotate90deg(image, sign == 'r');
        std::swap(height, width);
    }
    else {
        std::cerr << "Effect: \"" << effect << "\" not available\n";
        return false;
    }

    rgbWrite(width, height, image, output);

    return true;
}
