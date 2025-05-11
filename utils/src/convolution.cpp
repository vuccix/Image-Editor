#include "../convolution.h"

RGB_IMAGE convolution3x3(const RGB_IMAGE& img, const float kernel[3][3]) {
    const int rows = static_cast<int>(img.size());
    const int cols = static_cast<int>(img[0].size());
    RGB_IMAGE result(rows, std::vector<PIXEL>(cols));

    for (int i{}; i < rows; ++i) {
        for (int j{}; j < cols; ++j) {
            float gradient[3] = { 0.f, 0.f, 0.f };

            for (int ki = -1; ki < 2; ++ki)
                for (int kj = -1; kj < 2; ++kj) {
                    int idX = (i + ki < 0 || i + ki > rows - 1) ? i - ki : i + ki;
                    int idY = (j + kj < 0 || j + kj > cols - 1) ? j - kj : j + kj;

                    float r = img[idX][idY][0];
                    float g = img[idX][idY][1];
                    float b = img[idX][idY][2];

                    gradient[0] += kernel[ki + 1][kj + 1] * r;
                    gradient[1] += kernel[ki + 1][kj + 1] * g;
                    gradient[2] += kernel[ki + 1][kj + 1] * b;
                }

            int new_r = static_cast<int>(gradient[0]);
            int new_g = static_cast<int>(gradient[1]);
            int new_b = static_cast<int>(gradient[2]);

            result[i][j] = {
                    static_cast<uint8_t>(std::min(255, new_r)),
                    static_cast<uint8_t>(std::min(255, new_g)),
                    static_cast<uint8_t>(std::min(255, new_b))
            };
        }
    }

    return result;
}

GRAYSCALE convolution3x3(const GRAYSCALE& img, const float kernel[3][3]) {
    const int rows = static_cast<int>(img.size());
    const int cols = static_cast<int>(img[0].size());
    GRAYSCALE result(rows, std::vector<float>(cols, 0.f));

    // convolution
    for (int i{}; i < rows; ++i) {
        for (int j{}; j < cols; ++j) {
            float gradient = 0.f;

            for (int ki = -1; ki < 2; ++ki)
                for (int kj = -1; kj < 2; ++kj) {
                    int idX = (i + ki < 0 || i + ki > rows - 1) ? i - ki : i + ki;
                    int idY = (j + kj < 0 || j + kj > cols - 1) ? j - kj : j + kj;
                    gradient += kernel[ki + 1][kj + 1] * img[idX][idY];
                }

            result[i][j] = gradient;
        }
    }

    return result;
}

RGB_IMAGE convolution5x5(const RGB_IMAGE& img, const float kernel[5][5]) {
    const int rows = static_cast<int>(img.size());
    const int cols = static_cast<int>(img[0].size());
    RGB_IMAGE result(rows, std::vector<PIXEL>(cols));

    for (int i{}; i < rows; ++i) {
        for (int j{}; j < cols; ++j) {
            float gradient[3] = { 0.f, 0.f, 0.f };

            for (int ki = -2; ki < 3; ++ki)
                for (int kj = -2; kj < 3; ++kj) {
                    int idX = (i + ki < 0 || i + ki > rows - 1) ? i - ki : i + ki;
                    int idY = (j + kj < 0 || j + kj > cols - 1) ? j - kj : j + kj;

                    float r = img[idX][idY][0];
                    float g = img[idX][idY][1];
                    float b = img[idX][idY][2];

                    gradient[0] += kernel[ki + 2][kj + 2] * r;
                    gradient[1] += kernel[ki + 2][kj + 2] * g;
                    gradient[2] += kernel[ki + 2][kj + 2] * b;
                }

            int new_r = static_cast<int>(gradient[0]);
            int new_g = static_cast<int>(gradient[1]);
            int new_b = static_cast<int>(gradient[2]);

            result[i][j] = {
                    static_cast<uint8_t>(std::min(255, new_r)),
                    static_cast<uint8_t>(std::min(255, new_g)),
                    static_cast<uint8_t>(std::min(255, new_b))
            };
        }
    }

    return result;
}

GRAYSCALE convolution5x5(const GRAYSCALE& img, const float kernel[5][5]) {
    const int rows = static_cast<int>(img.size());
    const int cols = static_cast<int>(img[0].size());
    GRAYSCALE result(rows, std::vector<float>(cols, 0.f));

    // convolution
    for (int i{}; i < rows; ++i) {
        for (int j{}; j < cols; ++j) {
            float gradient = 0.f;

            for (int ki = -2; ki < 3; ++ki)
                for (int kj = -2; kj < 3; ++kj) {
                    int idX = (i + ki < 0 || i + ki > rows - 1) ? i - ki : i + ki;
                    int idY = (j + kj < 0 || j + kj > cols - 1) ? j - kj : j + kj;
                    gradient += kernel[ki + 2][kj + 2] * img[idX][idY];
                }

            result[i][j] = gradient;
        }
    }

    return result;
}
