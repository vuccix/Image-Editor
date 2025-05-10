#include "../effects.h"

GRAYSCALE convert_1D(const RGB_IMAGE& img, const float arr[3]) {
    const int rows = static_cast<int>(img.size());
    const int cols = static_cast<int>(img[0].size());
    GRAYSCALE result(rows, std::vector<float>(cols));

    for (int i{}; i < rows; ++i)
        for (int j{}; j < cols; ++j) {
            float r = img[i][j][0];
            float g = img[i][j][1];
            float b = img[i][j][2];

            result[i][j] = (arr[0] * r) + (arr[1] * g) + (arr[2] * b);
        }

    return result;
}

void convert_2D(RGB_IMAGE& img, const float arr[3][3]) {
    const int rows = static_cast<int>(img.size());
    const int cols = static_cast<int>(img[0].size());

    for (int i{}; i < rows; ++i)
        for (int j{}; j < cols; ++j) {
            float r = img[i][j][0];
            float g = img[i][j][1];
            float b = img[i][j][2];

            int new_r = static_cast<int>(arr[0][0] * r + arr[0][1] * g + arr[0][2] * b);
            int new_g = static_cast<int>(arr[1][0] * r + arr[1][1] * g + arr[1][2] * b);
            int new_b = static_cast<int>(arr[2][0] * r + arr[2][1] * g + arr[2][2] * b);

            img[i][j][0] = static_cast<uint8_t>(std::min(255, new_r));
            img[i][j][1] = static_cast<uint8_t>(std::min(255, new_g));
            img[i][j][2] = static_cast<uint8_t>(std::min(255, new_b));
        }
}

GRAYSCALE Effects::grayscale(const RGB_IMAGE &img) {
    static constexpr float arr[3] = { 0.2989f, 0.5870f, 0.1140f };
    return convert_1D(img, arr);
}

LUMINANCE Effects::luminance(const RGB_IMAGE &img) {
    static constexpr float arr[3] = { 0.2126f, 0.7152f, 0.0722f };
    return convert_1D(img, arr);
}

void Effects::sepia(RGB_IMAGE &img) {
    static constexpr float arr[3][3] = {
        { 0.393f, 0.769f, 0.189f },
        { 0.349f, 0.686f, 0.168f },
        { 0.272f, 0.534f, 0.131f }
    };
    convert_2D(img, arr);
}
