#include "../effects.h"
#include "../../utils/sobel.h"
#include <cmath>

GRAYSCALE Effects::cannyEdge(RGB_IMAGE& img) {
    const int rows = static_cast<int>(img.size());
    const int cols = static_cast<int>(img[0].size());

    // apply Gaussian blur
    Effects::blur5x5(img);
    GRAYSCALE gray = grayscale(img);

    // compute gradients
    const GRAYSCALE Gx = sobel(gray, false);
    const GRAYSCALE Gy = sobel(gray, true);

    // compute gradient magnitude and direction
    GRAYSCALE magnitude(rows, std::vector<float>(cols));
    GRAYSCALE direction(rows, std::vector<float>(cols));

    for (int i{}; i < rows; ++i)
        for (int j{}; j < cols; ++j) {
            magnitude[i][j] = std::hypot(Gx[i][j], Gy[i][j]);
            direction[i][j] = atan2f(Gy[i][j], Gx[i][j]) * (180.f / M_PI);

            if (direction[i][j] < 0) direction[i][j] += 180.f;
        }

    // non-maximum suppression
    GRAYSCALE nonMax(rows, std::vector<float>(cols, 0.f));
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            float angle = direction[i][j];
            float mag   = magnitude[i][j];
            float q     = 0.f;
            float r     = 0.f;

            // interpolate by gradient direction
            if ((0 <= angle && angle < 22.5f) || (157.5f <= angle && angle <= 180.f)) {
                q = magnitude[i][j + 1];
                r = magnitude[i][j - 1];
            }
            else if (22.5f <= angle && angle < 67.5f) {
                q = magnitude[i - 1][j + 1];
                r = magnitude[i + 1][j - 1];
            }
            else if (67.5f <= angle && angle < 112.5f) {
                q = magnitude[i - 1][j];
                r = magnitude[i + 1][j];
            }
            else if (112.5f <= angle && angle < 157.5f) {
                q = magnitude[i - 1][j - 1];
                r = magnitude[i + 1][j + 1];
            }

            if (mag >= q && mag >= r)
                nonMax[i][j] = mag;
            else
                nonMax[i][j] = 0;
        }
    }

    // double thresholding
    const float lowThresh  = 20.f;
    const float highThresh = 50.f;
    GRAYSCALE edges(rows, std::vector<float>(cols, 0.f));

    for (int i{}; i < rows; ++i)
        for (int j{}; j < cols; ++j) {
            float val = nonMax[i][j];

            if (val >= highThresh)
                edges[i][j] = 255.f; // strong
            else if (val >= lowThresh)
                edges[i][j] = 128.f; // weak
            else
                edges[i][j] = 0.f;   // non-edge
        }

    // hysteresis
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j)
            if (edges[i][j] == 128.f) {
                bool connectedToStrong = false;

                for (int dx = -1; dx <= 1 && !connectedToStrong; ++dx)
                    for (int dy = -1; dy <= 1; ++dy)
                        if (edges[i + dx][j + dy] == 255.f)
                            connectedToStrong = true;

                edges[i][j] = connectedToStrong ? 255.f : 0.f;
            }
    }

    return edges;
}
