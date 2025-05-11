#include "../effects.h"

void Effects::invert(RGB_IMAGE& img) {
    for (auto& row : img) {
        for (PIXEL& pixel : row) {
            pixel[0] = 255 - pixel[0];
            pixel[1] = 255 - pixel[1];
            pixel[2] = 255 - pixel[2];
        }
    }
}

RGB_IMAGE Effects::rotate90deg(const RGB_IMAGE& img, bool clockwise) {
    size_t rows = img.size();
    size_t cols = img[0].size();

    RGB_IMAGE res(cols, std::vector<PIXEL>(rows));
    if (clockwise) {
        for (size_t i{}; i < rows; ++i)
            for (size_t j{}; j < cols; ++j)
                res[j][rows - 1 - i] = img[i][j];
    }
    // counterclockwise
    else {
        for (size_t i{}; i < rows; ++i)
            for (size_t j{}; j < cols; ++j)
                res[cols - 1 - j][i] = img[i][j];
    }

    return res;
}

void Effects::channelSwap(RGB_IMAGE& img) {
    for (auto& i : img)
        for (PIXEL& pix : i) { // RGB -> BRG
            std::swap(pix[0], pix[1]); // R <-> G
            std::swap(pix[0], pix[2]); // G <-> B
        }
}

void Effects::brightness(RGB_IMAGE& img, int value) {
    for (auto& i : img) {
        for (PIXEL& pix : i) {
            int r = value + pix[0];
            int g = value + pix[1];
            int b = value + pix[2];

            int new_r = (value >= 0) ? std::min(255, r) : std::max(0, r);
            int new_g = (value >= 0) ? std::min(255, g) : std::max(0, g);
            int new_b = (value >= 0) ? std::min(255, b) : std::max(0, b);

            pix[0] = static_cast<uint8_t>(new_r);
            pix[1] = static_cast<uint8_t>(new_g);
            pix[2] = static_cast<uint8_t>(new_b);
        }
    }
}

void Effects::duotone(RGB_IMAGE& img) {
    PIXEL high = { 16, 248, 135 };
    PIXEL low  = { 11,  24,  75 };

    GRAYSCALE gray = grayscale(img);

    for (size_t i = 0; i < img.size(); ++i) {
        for (size_t j = 0; j < img[0].size(); ++j) {
            float intensity = gray[i][j] / 255.f;

            PIXEL& pix = img[i][j];
            pix[0] = static_cast<uint8_t>((1.f - intensity) * low[0] + intensity * high[0]);
            pix[1] = static_cast<uint8_t>((1.f - intensity) * low[1] + intensity * high[1]);
            pix[2] = static_cast<uint8_t>((1.f - intensity) * low[2] + intensity * high[2]);
        }
    }
}
