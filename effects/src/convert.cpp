#include "../convert.h"

GRAYSCALE convert_1D(const RGB_IMAGE& img, const float arr[3]) {
    const int rows = static_cast<int>(img.size());
    const int cols = static_cast<int>(img[0].size());
    GRAYSCALE result(rows, std::vector<float>(cols));

    for (int i{}; i < rows; ++i)
        for (int j{}; j < cols; ++j)
            result[i][j] = (arr[0] * img[i][j][0]) + (arr[1] * img[i][j][1]) + (arr[2] * img[i][j][2]);

    return result;
}

RGB_IMAGE convert_2D(const RGB_IMAGE& img, const float arr[3][3]) {
    const int rows = static_cast<int>(img.size());
    const int cols = static_cast<int>(img[0].size());
    RGB_IMAGE result(rows, std::vector<std::array<int, 3>>(cols));

    for (int i{}; i < rows; ++i)
        for (int j{}; j < cols; ++j) {
            result[i][j][0] = (arr[0][0] * img[i][j][0]) + (arr[0][1] * img[i][j][1]) + (arr[0][2] * img[i][j][2]);
            result[i][j][1] = (arr[1][0] * img[i][j][0]) + (arr[1][1] * img[i][j][1]) + (arr[1][2] * img[i][j][2]);
            result[i][j][2] = (arr[2][0] * img[i][j][0]) + (arr[2][1] * img[i][j][1]) + (arr[2][2] * img[i][j][2]);
        }

    return result;
}

GRAYSCALE Convert::grayscale(const RGB_IMAGE &img) {
    static constexpr float arr[3] = { 0.2989f, 0.5870f, 0.1140f };
    return convert_1D(img, arr);
}

LUMINANCE Convert::luminance(const RGB_IMAGE &img) {
    static constexpr float arr[3] = { 0.2126f, 0.7152f, 0.0722f };
    return convert_1D(img, arr);
}

SEPIA Convert::sepia(const RGB_IMAGE &img) {
    static constexpr float arr[3][3] = {
        { 0.393f, 0.769f, 0.189f },
        { 0.349f, 0.686f, 0.168f },
        { 0.272f, 0.534f, 0.131f }
    };
    return convert_2D(img, arr);
}
