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

RGB_IMAGE Effects::rotate90deg(const RGB_IMAGE &img, bool clockwise) {
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
